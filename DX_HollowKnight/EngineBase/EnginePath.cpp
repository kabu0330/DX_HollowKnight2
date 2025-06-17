#include "PreCompile.h"
#include "EnginePath.h"
#include "EngineDebug.h"

UEnginePath::UEnginePath()
	: Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::string_view _Path)
	: Path(_Path)
{

}

UEnginePath::UEnginePath(std::filesystem::path _Path)
	: Path(_Path)
{

}

UEnginePath::~UEnginePath()
{
}

std::string UEnginePath::GetPathToString()
{
	return Path.string();
}

std::string UEnginePath::GetFileNameToString()
{
	if (true == IsDirectory())
	{
		MSGASSERT("파일 경로가 잘못되었습니다." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetDirectoryNameToString()
{
	if (false == IsDirectory())
	{
		MSGASSERT("디렉토리 경로가 잘못되었습니다. " + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetExtensionToString()
{
	return Path.extension().string();
}

bool UEnginePath::DoesPathExist()
{
	return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
	return false == IsDirectory();
}

bool UEnginePath::IsDirectory()
{
	return std::filesystem::is_directory(Path);
}

void UEnginePath::MoveParentPath()
{
	Path = Path.parent_path();
}

bool UEnginePath::MovePath(std::string_view _Path)
{
	Append(_Path);

	if (false == DoesPathExist())
	{
		MSGASSERT("존재하지 않는 경로로 이동할 수 없습니다."); 
		return false;
	}

	return true;
}

void UEnginePath::Append(std::string_view _AppendName)
{
	Path.append(_AppendName);
}

bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
	// Path = L"D:\\Project\\GM2\\API\\App\\AAA.png"
	UEnginePath DummyPath = UEnginePath(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로가 아닙니다.");
		return false;
	}

	bool Result = false;
	std::filesystem::path CurPath = DummyPath.Path;

	std::filesystem::path Root = CurPath.root_path();
	while (true)
	{
		CurPath = DummyPath.Path;

		if (CurPath == Root)
		{
			break;
		}

		CurPath.append(_Path);
		if (true == std::filesystem::exists(CurPath))
		{
			Result = true;
			Path = CurPath;
			break;
		}
		DummyPath.MoveParentPath();
	}

	return Result;
}



