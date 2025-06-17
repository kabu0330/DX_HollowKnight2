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
		MSGASSERT("���� ��ΰ� �߸��Ǿ����ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetDirectoryNameToString()
{
	if (false == IsDirectory())
	{
		MSGASSERT("���丮 ��ΰ� �߸��Ǿ����ϴ�. " + Path.string());
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
		MSGASSERT("�������� �ʴ� ��η� �̵��� �� �����ϴ�."); 
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
		MSGASSERT("���丮 ��ΰ� �ƴմϴ�.");
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



