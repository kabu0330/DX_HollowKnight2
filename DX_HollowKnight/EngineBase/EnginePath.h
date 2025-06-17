#pragma once
#include <filesystem>

// ���� :
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

	// ���ϸ� + Ȯ���� ����
	ENGINEAPI std::string GetFileNameToString();

	// ���ϸ� + Ȯ���� ����
	ENGINEAPI std::string GetDirectoryNameToString();

	// Ȯ����
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

