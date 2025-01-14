#include "PreCompile.h"
#include "ContentsResource.h"

void UContentsResource::LoadResource()
{
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}
		Dir.Append("Image");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::Load(FilePath);
		}
	}

	{
		// �� ���ҽ�
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("MapObjectResources");

		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::Load(FilePath);
		}
	}

	{
		// �� ���ҽ�
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("MapData");

		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::Load(FilePath);
		}
	}
}

void UContentsResource::LoadContentsResource(std::string_view _Path)
{
	std::string Path = _Path.data();
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append(Path);
	UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
}

void UContentsResource::LoadResourceDirectory()
{
	LoadResource(); // ���� 1ȸ ���ҽ� ������ �ε��ؾ� �Ѵ�.

	// Knight
	LoadContentsResource("Image/Knight/Idle");
	LoadContentsResource("Image/Knight/Run");
	LoadContentsResource("Image/Knight/Jump");
	LoadContentsResource("Image/Knight/RunToIdle");
	LoadContentsResource("Image/Knight/IdleToRun");
	LoadContentsResource("Image/Knight/Slash");
	LoadContentsResource("Image/Knight/UpSlash");
	LoadContentsResource("Image/Knight/DownSlash");
	LoadContentsResource("Image/Knight/Damage");
	LoadContentsResource("Image/Knight/Death");
	LoadContentsResource("Image/Knight/DeathDamage");
	LoadContentsResource("Image/Knight/DeathHead");
	LoadContentsResource("Image/Knight/Airborn");
	LoadContentsResource("Image/Knight/Land");
	LoadContentsResource("Image/Knight/HardLand");
	LoadContentsResource("Image/Knight/LookDown");
	LoadContentsResource("Image/Knight/LookDownLoop");
	LoadContentsResource("Image/Knight/LookUp");
	LoadContentsResource("Image/Knight/LookUpLoop");
	LoadContentsResource("Image/Knight/Dash");
	LoadContentsResource("Image/Knight/Focus");
	LoadContentsResource("Image/Knight/FocusGet");
	LoadContentsResource("Image/Knight/FocusEnd");
	LoadContentsResource("Image/Knight/FireballAntic");
	LoadContentsResource("Image/Knight/FireballCast");

	// Knight Effect
	LoadContentsResource("Image/Effect/Knight/SlashEffect");
	LoadContentsResource("Image/Effect/Knight/UpSlashEffect");
	LoadContentsResource("Image/Effect/Knight/DownSlashEffect");
	LoadContentsResource("Image/Effect/Knight/FocusEffect");
	LoadContentsResource("Image/Effect/Knight/FocusEffectEnd");
}

void UContentsResource::LoadFolder()
{
	//UEngineDirectory TitleMain;
	//TitleMain.MoveParentToDirectory("ContentsResources//Image//Title");
	//TitleMain.Append("TitleBackGround");
}

void UContentsResource::LoadSprite()
{
	UEngineSprite::CreateSpriteToMeta("Knight_Idle.png", ".smeta");
}

void UContentsResource::LoadShaderResource()
{
	// Shader
	{
		UEngineDirectory CurDir;
		CurDir.MoveParentToDirectory("ContentsShader");

		std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

		for (size_t i = 0; i < ShaderFiles.size(); i++)
		{
			UEngineShader::ReflectionCompile(ShaderFiles[i]);
		}
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyMaterial");
		Mat->SetVertexShader("ContentsShader.fx");
		Mat->SetPixelShader("ContentsShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyCollisionDebugMaterial");
		Mat->SetVertexShader("EngineDebugCollisionShader.fx");
		Mat->SetPixelShader("EngineDebugCollisionShader.fx");
		// ������ ȭ�鿡 ������ ������ �̳༮�� ���� ������ ����.
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
	}
}

UContentsResource::UContentsResource()
{
}

UContentsResource::~UContentsResource()
{
}