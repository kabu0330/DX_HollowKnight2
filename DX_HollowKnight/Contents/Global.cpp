#include "PreCompile.h"
#include "Global.h"
#include <EnginePlatform/EngineWinImage.h>

UEngineWinImage* GlobalFunc::LoadResource(std::string_view _Directory, std::string_view _SpriteName, UEngineWinImage* _ResImage)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return nullptr;
	}
	Dir.Append(_Directory);
	UEngineFile ImageFiles = Dir.GetFile(_SpriteName);

	_ResImage->Load(nullptr, ImageFiles.GetPathToString());

	return _ResImage;
}
