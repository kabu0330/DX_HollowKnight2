#pragma once
#include <filesystem>

// 설명 :
class UEnginePath
{
public:
	ENGINEAPI UEnginePath();
	ENGINEAPI UEnginePath(std::string_view _Path);
	ENGINEAPI UEnginePath(std::filesystem::path _Path);
	ENGINEAPI virtual ~UEnginePath();

	ENGINEAPI bool DoesPathExist();
	ENGINEAPI void MoveParentPath();

	ENGINEAPI std::string GetPathToString();

	// 파일명 + 확장자 포함
	ENGINEAPI std::string GetFileNameToString();

	// 파일명 + 확장자 포함
	ENGINEAPI std::string GetDirectoryNameToString();

	// 확장자
	ENGINEAPI std::string GetExtensionToString();


	ENGINEAPI bool MoveParentToDirectory(std::string_view _Path);

	ENGINEAPI bool MovePath(std::string_view _Path);

	ENGINEAPI bool IsDirectory();

	ENGINEAPI bool IsFile();

	ENGINEAPI void Append(std::string_view _AppendName);

protected:
	std::filesystem::path Path;

private:
};

