#pragma once

#include "fileInterface.h"

#include <morfuse/Container/Container.h>
#include <morfuse/Common/Time.h>
#include <morfuse/Script/Event.h>

#include <filesystem>

struct execParam_t {
	mfuse::uinttime_t maxExecTime;
	uint16_t frameRate;
};

class ScriptInstance
{
public:
	ScriptInstance(const std::filesystem::path& dir);
	~ScriptInstance();

	void initEvents(const std::filesystem::path& fileName);
	void execute(const char* fname, const execParam_t& param);
	void dumpClasses(std::ostream& out);

private:
	FileManagement fileManagement;

	struct PrivateEvent
	{
		PrivateEvent(const std::string& nameRef);

		std::string name;
		mfuse::EventDef def[2];
	};
	mfuse::con::Container<PrivateEvent> eventList;
};

