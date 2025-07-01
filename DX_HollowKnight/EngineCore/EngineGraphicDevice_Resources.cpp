#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineVertex.h"
#include "EngineVertexBuffer.h"
#include "EngineIndexBuffer.h"
#include "Mesh.h"
#include "EngineBlend.h"
#include "EngineShader.h"
#include "EngineMaterial.h"
#include "EngineTexture.h"
#include "EngineFont.h"
#include "EngineDepthStencilState.h"

void UEngineGraphicDevice::InitDefaultResources()
{
	InitDepthStencil();
	InitTexture();
	InitMesh();
	InitBlend();
	InitRasterizerState();
	InitShader();
	InitMaterial();

	UEngineFont::LoadFont("�ü�", "�ü�");
}

void UEngineGraphicDevice::InitDepthStencil() 
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("BaseDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("UIDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("CollisionDebugDepth", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;

		UEngineDepthStencilState::Create("TargetMerge", Desc);
	}
}

void UEngineGraphicDevice::InitTexture()
{

		D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
		SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // 0~1���̸� ��ȿ
		SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP; // y
		SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // z // 3�� 

		SampInfo.BorderColor[0] = 0.0f;
		SampInfo.BorderColor[1] = 0.0f;
		SampInfo.BorderColor[2] = 0.0f;
		SampInfo.BorderColor[3] = 0.0f;

		UEngineSampler::Create("WRAPSampler", SampInfo);

	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("EngineShader"))
		{
			MSGASSERT("EngineShader ���� ��ȸ�� �����߽��ϴ�.");
			return;
		}
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::LoadTexture(FilePath);
		}
	}
}

void UEngineGraphicDevice::InitShader()
{
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("EngineShader");

	std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, {".fx", ".hlsl"});

	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(ShaderFiles[i]);
	}
}

void UEngineGraphicDevice::InitMesh()
{
	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = FEngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(0.5f, -0.5f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("Rect", Vertexs);
	}
	{
		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		Indexs.push_back(1);
		Indexs.push_back(3);
		Indexs.push_back(2);
		UEngineIndexBuffer::Create("Rect", Indexs);
	}
	// �������� 1�� �ϸ� ȭ�� ��ü�� ������ �޽ø� ���� �� �ִ�.
	{
		std::vector<FEngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = FEngineVertex{ FVector(-1.0f, 1.0f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = FEngineVertex{ FVector(1.0f, 1.0f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = FEngineVertex{ FVector(-1.0f, -1.0f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = FEngineVertex{ FVector(1.0f, -1.0f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UEngineVertexBuffer::Create("FullRect", Vertexs);
	}
	{
		UMesh::Create("Rect");

		UMesh::CreateWithBuffers("FullRect", "FullRect", "Rect");
	}
}

void UEngineGraphicDevice::InitBlend()
{
	D3D11_BLEND_DESC Desc = {0};

	Desc.AlphaToCoverageEnable = false;			// ��Ƽ���ø� ���� Ŀ������
	Desc.IndependentBlendEnable = true;			// �� ���� Ÿ�ٺ��� ���� ���θ� ����
	Desc.RenderTarget[0].BlendEnable = true;	// ���� Ȱ��ȭ
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // RGBA ��� ���

	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // ���� �߰� ������ '+'

	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			// �ҽ� ���İ� ���
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		// ��� ���İ� ���� ��� (1 - �ҽ� ���İ�) = ��� ���İ�

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			// ���� ����
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			// ���� �ҽ� ��
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;			// ���� ��� ��

	UEngineBlend::Create("AlphaBlend", Desc);
}

void UEngineGraphicDevice::InitRasterizerState()
{
	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		UEngineRasterizerState::Create("EngineBase", Desc);
	}
	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		UEngineRasterizerState::Create("CollisionDebugRasterizer", Desc);
	}
}

void UEngineGraphicDevice::InitMaterial()
{
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("CollisionDebugMaterial");
		Mat->SetVertexShader("EngineDebugCollisionShader.fx");
		Mat->SetPixelShader("EngineDebugCollisionShader.fx");
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRasterizer");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("WidgetMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetDepthStencilState("UIDepth");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("TargetMerge");
		Mat->SetVertexShader("EngineTargetMergeShader.fx");
		Mat->SetPixelShader("EngineTargetMergeShader.fx");
		Mat->SetDepthStencilState("TargetMerge"); 
	}
}