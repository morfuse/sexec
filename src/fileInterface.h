#pragma once

#include <morfuse/Script/interfaces/file.h>

#include <filesystem>
#include <fstream>

class FileManagement : public mfuse::IFileManagement
{
public:
	FileManagement(const std::filesystem::path& baseDirValue);
	mfuse::IFile* OpenFile(const char* fname) override;
	void CloseFile(mfuse::IFile* file) noexcept override;

private:
	std::filesystem::path baseDir;
};

class File : public mfuse::IFile
{
public:
	File(const std::filesystem::path& filePath);

	std::istream& getStream() noexcept;

private:
	std::fstream stream;
};
