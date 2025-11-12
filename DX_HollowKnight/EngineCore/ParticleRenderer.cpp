#include "PreCompile.h"
#include "ParticleRenderer.h"
#include "EngineCore.h"
#include "EngineGraphicDevice.h"
#include "EngineVertexShader.h"
#include "EngineGeometryShader.h"
#include "EnginePixelShader.h"
#include "EngineTexture.h"
#include "EngineCamera.h"
#include <EngineBase/EngineDebug.h>

UParticleRenderer::UParticleRenderer()
{
}

UParticleRenderer::~UParticleRenderer()
{
}

void UParticleRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

void UParticleRenderer::ComponentTick(float _DeltaTime)
{
	URenderer::ComponentTick(_DeltaTime);
}

void UParticleRenderer::Initialize(UINT _MaxParticles)
{
	MaxParticles = _MaxParticles;
	SystemData.MaxParticles = static_cast<float>(_MaxParticles);

	Particles.reserve(MaxParticles);
	Velocities.reserve(MaxParticles);

	CreateParticleBuffer();

	IsInitialized = true;
}

void UParticleRenderer::CreateParticleBuffer()
{
	if (MaxParticles == 0)
	{
		MSGASSERT("MaxParticles가 0입니다. Initialize를 먼저 호출하세요.");
		return;
	}

	// 동적 정점 버퍼 생성
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(FGPUParticle) * MaxParticles;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = UEngineCore::GetDevice().GetDevice()->CreateBuffer(&bufferDesc, nullptr, &ParticleVertexBuffer);

	if (FAILED(hr))
	{
		MSGASSERT("파티클 정점 버퍼 생성 실패");
	}
}

void UParticleRenderer::AddParticle(const FVector& _Position, const FVector& _Velocity, const FVector2D& _Size,
	const FVector& _Color, float _Alpha, float _LifeTime)
{
	if (Particles.size() >= MaxParticles)
	{
		return; // 최대 파티클 수 도달
	}

	FGPUParticle newParticle;
	newParticle.Position = _Position;
	newParticle.Size = _Size;
	newParticle.Color = _Color;
	newParticle.Alpha = _Alpha;
	newParticle.Age = 0.0f;
	newParticle.LifeTime = _LifeTime;

	Particles.push_back(newParticle);
	Velocities.push_back(_Velocity);
}

void UParticleRenderer::UpdateParticles(float _DeltaTime)
{
	if (Particles.empty())
	{
		return;
	}

	// 파티클 업데이트 (CPU)
	for (size_t i = 0; i < Particles.size(); )
	{
		Particles[i].Age += _DeltaTime;

		// 죽은 파티클 제거
		if (Particles[i].Age > Particles[i].LifeTime)
		{
			Particles[i] = Particles.back();
			Particles.pop_back();

			Velocities[i] = Velocities.back();
			Velocities.pop_back();
			continue;
		}

		// 물리 업데이트
		Velocities[i] += SystemData.Gravity * _DeltaTime;
		Particles[i].Position += Velocities[i] * _DeltaTime;

		++i;
	}

	// GPU 버퍼 업데이트
	UpdateParticleBuffer();
}

void UParticleRenderer::UpdateParticleBuffer()
{
	if (nullptr == ParticleVertexBuffer || Particles.empty())
	{
		return;
	}

	// 정점 버퍼 업데이트
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = UEngineCore::GetDevice().GetContext()->Map(
		ParticleVertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);

	if (FAILED(hr))
	{
		MSGASSERT("파티클 버퍼 맵핑 실패");
		return;
	}

	// 데이터 복사
	memcpy(mappedResource.pData, Particles.data(), sizeof(FGPUParticle) * Particles.size());

	UEngineCore::GetDevice().GetContext()->Unmap(ParticleVertexBuffer.Get(), 0);
}

void UParticleRenderer::SetParticleShader(std::string_view _VSName, std::string_view _GSName, std::string_view _PSName)
{
	VertexShaderName = _VSName;
	GeometryShaderName = _GSName;
	PixelShaderName = _PSName;
}

void UParticleRenderer::SetParticleTexture(std::string_view _TextureName)
{
	TextureName = _TextureName;
}

void UParticleRenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	if (false == IsInitialized || Particles.empty() || nullptr == ParticleVertexBuffer)
	{
		return;
	}

	// 카메라 위치 업데이트
	SystemData.CameraPosition = _Camera->GetActorLocation();
	SystemData.DeltaTime = _DeltaTime;

	ID3D11DeviceContext* Context = UEngineCore::GetDevice().GetContext();

	// 입력 레이아웃 설정은 생략 (기본 설정 사용)

	// 정점 버퍼 바인딩
	UINT stride = sizeof(FGPUParticle);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, ParticleVertexBuffer.GetAddressOf(), &stride, &offset);

	// 프리미티브 토폴로지 설정 (포인트 리스트)
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 셰이더 바인딩
	std::shared_ptr<UEngineVertexShader> VS = UEngineVertexShader::Find<UEngineVertexShader>(VertexShaderName);
	std::shared_ptr<UEngineGeometryShader> GS = UEngineGeometryShader::Find<UEngineGeometryShader>(GeometryShaderName);
	std::shared_ptr<UEnginePixelShader> PS = UEnginePixelShader::Find<UEnginePixelShader>(PixelShaderName);

	if (nullptr != VS)
	{
		VS->VSSetShader();
	}

	if (nullptr != GS)
	{
		GS->GSSetShader();
	}

	if (nullptr != PS)
	{
		PS->PSSetShader();
	}

	// 텍스처 바인딩
	if (false == TextureName.empty())
	{
		std::shared_ptr<UEngineTexture> Texture = UEngineTexture::Find<UEngineTexture>(TextureName);
		if (nullptr != Texture)
		{
			Texture->PSSetTexture(0);
		}
	}

	// 상수 버퍼 업데이트 및 바인딩은 별도 구현 필요
	// (FTransform, FParticleSystemData 등)

	// 드로우 콜 (한 번에 모든 파티클 렌더링!)
	Context->Draw(static_cast<UINT>(Particles.size()), 0);

	// 지오메트리 셰이더 언바인딩
	if (nullptr != GS)
	{
		GS->GSUnsetShader();
	}
}
