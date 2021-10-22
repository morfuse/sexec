#pragma once

#include <iostream>

class LogBuffer : public std::streambuf
{
public:
	LogBuffer(const char* namePtr, std::ostream& outRef);
	int_type overflow(std::streambuf::int_type c) override;

private:
	const char* name;
	std::ostream& out;
	bool announce;
};

class LogStream : public std::ostream
{
public:
	LogStream(const char* name, std::ostream& out);

private:
	LogBuffer buf;
};

extern LogStream debugStream;
extern LogStream outputStream;
extern LogStream warnStream;
extern LogStream errorStream;
