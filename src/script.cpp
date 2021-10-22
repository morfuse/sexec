#include "script.h"
#include "log.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include <morfuse/Script/Context.h>
#include <morfuse/Script/ClassEventPrinter.h>

using namespace mfuse;
namespace fs = std::filesystem;

ScriptInstance::ScriptInstance(const std::filesystem::path& dir)
	: fileManagement(dir)
{

}

ScriptInstance::~ScriptInstance()
{
}


ScriptInstance::PrivateEvent::PrivateEvent(const std::string& nameRef)
	: name(nameRef)
	, def{ EventDef(name.c_str(), 0, "", "", "", evType_e::Normal), EventDef(name.c_str(), 0, "", "", "", evType_e::Return) }
{

}

void ScriptInstance::initEvents(const std::filesystem::path& fileName)
{
	std::ifstream file(fileName, std::ifstream::in | std::ifstream::binary);

	if (!file.is_open())
	{
		std::cerr << "a file name was specified for the event list: " << fileName << " but it is invalid!" << std::endl;
		return;
	}

	// calculate the number of lines and preallocate
	size_t numLines = 0;
	bool nonLine = false;
	for (auto it = std::istreambuf_iterator<char>(file); it != std::istreambuf_iterator<char>(); ++it)
	{
		if (*it != '\n')
		{
			if (!nonLine)
			{
				++numLines;
				nonLine = true;
			}
		}
		else
		{
			nonLine = false;
		}
	}

	//const size_t numLines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	if (!numLines)
	{
		// there is nothing in it, silent success
		return;
	}

	// just allocate a buffer without preallocating
	const size_t numEvents = numLines;
	eventList.Resize(numEvents);

	file.seekg(0);

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			// ignore empty lines
			continue;
		}

		new (eventList) PrivateEvent(line);
	}

	assert(eventList.NumObjects() == numEvents);
}

void ScriptInstance::execute(const char* fname, const execParam_t& param)
{
	using namespace std::chrono;

	ScriptContext context;
	// enable dev to be able to check for line errors
	context.GetSettings().SetDeveloperEnabled(true);

	//context.GetOutputInfo().SetOutputStream(outputLevel_e::Verbose, &debugStream);
	context.GetOutputInfo().SetOutputStream(outputLevel_e::Debug, &debugStream);
	context.GetOutputInfo().SetOutputStream(outputLevel_e::Output, &outputStream);
	context.GetOutputInfo().SetOutputStream(outputLevel_e::Warn, &warnStream);
	context.GetOutputInfo().SetOutputStream(outputLevel_e::Error, &errorStream);

	context.GetScriptInterfaces().fileManagement = &fileManagement;

	IFile* const file = fileManagement.OpenFile(fname);

	ScriptMaster& Director = context.GetDirector();
	if (param.maxExecTime > 0)
	{
		ThreadExecutionProtection& threadProtection = Director.GetThreadExecutionProtection();
		threadProtection.SetLoopProtection(true);
		threadProtection.SetMaxExecutionTime(param.maxExecTime);
	}

	if (!fname || !*fname) {
		fname = "main.scr";
	}

	const ProgramScript* const script = context.GetDirector().GetProgramScript(fname, file->getStream());

	const TimeManager& timeManager = context.GetTimeManager();
	const uinttime_t startTime = timeManager.GetTime();
	// fire the script
	Director.ExecuteThread(script);

	const nanoseconds frameMs = nanoseconds(1000000000 / param.frameRate);

	while(!context.IsIdle())
	{
		const uinttime_t currentTime = timeManager.GetTime() - startTime;
		if (currentTime >= param.maxExecTime)
		{
			std::cerr << "Maximum execution time met";
			break;
		}

		context.Execute();

		std::this_thread::sleep_for(frameMs);
	}

	fileManagement.CloseFile(file);
}

void ScriptInstance::dumpClasses(std::ostream& out)
{
	ClassEventPrinter printer;

	HTMLFormater htmlFormater;
	printer.DumpAllClasses(out, htmlFormater, "App Classes");
}
