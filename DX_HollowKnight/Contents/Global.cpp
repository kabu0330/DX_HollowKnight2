#include "PreCompile.h"
#include "Global.h"
#include <EnginePlatform/EngineWinImage.h>

UEngineWinImage* GlobalFunc::LoadResource(std::string_view _Directory, std::string_view _SpriteName, UEngineWinImage* _ResImage)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return nullptr;
	}
	Dir.Append(_Directory);
	UEngineFile ImageFiles = Dir.GetFile(_SpriteName);

	_ResImage->Load(nullptr, ImageFiles.GetPathToString());

	return _ResImage;
}
