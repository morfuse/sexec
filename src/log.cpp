#include "log.h"


LogBuffer::LogBuffer(const char* namePtr, std::ostream& outRef)
	: name(namePtr)
	, out(outRef)
	, announce(true)
{}

std::streambuf::int_type LogBuffer::overflow(std::streambuf::int_type c)
{
	if (announce && c != '\n')
	{
		out << name << ": ";
		announce = false;
	}

	if (c == '\n') {
		announce = true;
	}

	out << char(c);
	return 0;
}

LogStream::LogStream(const char* name, std::ostream& out)
	: std::ostream(&buf)
	, buf(name, out)
{}

LogStream debugStream("D", std::cout);
LogStream outputStream("O", std::cout);
LogStream warnStream("W", std::cout);
LogStream errorStream("E", std::cerr);
