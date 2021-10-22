#include "script.h"
#include "log.h"

#include <string>
#include <filesystem>
#include <chrono>
#include <new>

#include <tclap/CmdLine.h>

namespace fs = std::filesystem;

int main(int argc, const char* argv[])
{
	using namespace std::chrono;
	const time_point<steady_clock> startTime = steady_clock::now();

	unsigned int result = 0;

	try
	{
		TCLAP::CmdLine cmd("Script Executor");

		TCLAP::ValueArg<std::string> dirArg("d", "dir", "Script directory.", false, "", "path");
		TCLAP::ValueArg<std::string> nameArg("s", "scr", "Script file relative to the script directory.", false, "", "string");
		TCLAP::ValueArg<mfuse::uinttime_t> maxExecTimeArg("t", "exectime", "Maximum script execution time (in milliseconds). Defaults to 0 for infinite.", false, 0, "integer");
		TCLAP::ValueArg<uint16_t> frameRateArg("r", "framerate", "Frame rate (Hz)", false, 20, "integer");
		TCLAP::ValueArg<std::string> eventListArg("e", "eventlist", "Path to the event list to grab", false, "", "path");
		TCLAP::SwitchArg dumpClasses("g", "doc", "Print documentation into an HTML file", cmd, false);

		cmd.add(dirArg);
		cmd.add(nameArg);
		cmd.add(maxExecTimeArg);
		cmd.add(frameRateArg);
		cmd.add(eventListArg);

		// parse the command line
		cmd.parse(argc, argv);

		fs::path dir = dirArg.getValue();

		if (dir.empty()) {
			dir = ".";
		}

		ScriptInstance instance(dir);

		const fs::path eventList = eventListArg.getValue();
		if (!eventList.empty())
		{
			// initialize the list of events to use during compilation
			instance.initEvents(eventList);
		}

		if (!dumpClasses.getValue())
		{
			execParam_t param;
			param.maxExecTime = maxExecTimeArg.getValue();
			param.frameRate = frameRateArg.getValue();

			if (!param.frameRate) {
				param.frameRate = 20;
			}

			// fire
			instance.execute(nameArg.getValue().c_str(), param);
		}
		else
		{
			// print to cout directly
			instance.dumpClasses(std::cout);
		}
	}
	catch (TCLAP::ArgException& e)
	{
		errorStream << "Failed to parse the command line: " << e.error() << " (arg " << e.argId() << ")" << std::endl;
		result = 1;
	}
	catch (std::exception& e)
	{
		errorStream << "Script execution failed: " << e.what() << std::endl;
		result = 1;
	}

	const milliseconds duration = duration_cast<milliseconds>(steady_clock::now() - startTime);
	//std::cout << "time: " << duration.count() << " ms" << std::endl;

	return result;
}