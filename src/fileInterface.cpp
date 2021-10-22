#include "fileInterface.h"

#include <fstream>

using namespace mfuse;
namespace fs = std::filesystem;

FileManagement::FileManagement(const std::filesystem::path& baseDirValue)
	: baseDir(baseDirValue)
{

}

mfuse::IFile* FileManagement::OpenFile(const char* fname)
{
	fs::path filePath = baseDir / fname;
	return new File(filePath);
}

void FileManagement::CloseFile(IFile* file) noexcept
{
	delete file;
}

File::File(const std::filesystem::path& filePath)
	: stream(filePath, std::ios::in | std::ios::binary)
{
	if (!stream.is_open()) {
		throw FileExceptions::NotFound(filePath.generic_string().c_str());
	}
}

std::istream& File::getStream() noexcept
{
	return stream;
}
