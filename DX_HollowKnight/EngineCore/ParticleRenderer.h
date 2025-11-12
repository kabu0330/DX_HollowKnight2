#pragma once
#include "Renderer.h"
#include <vector>

// 파티클 데이터 구조체 (GPU로 전송될 데이터)
struct FGPUParticle
{
	FVector Position = FVector::ZERO;    // 위치
	FVector2D Size = FVector2D(1.0f, 1.0f); // 크기
	float Padding1 = 0.0f;               // 16바이트 정렬을 위한 패딩
	FVector Color = FVector(1.0f, 1.0f, 1.0f); // RGB 색상
	float Alpha = 1.0f;                  // 알파
	float Age = 0.0f;                    // 현재 수명
	float LifeTime = 1.0f;               // 최대 수명
	float Padding2 = 0.0f;               // 패딩
	float Padding3 = 0.0f;               // 패딩
};

// 파티클 시스템 상수 버퍼
struct FParticleSystemData
{
	FVector CameraPosition = FVector::ZERO;
	float DeltaTime = 0.0f;
	FVector Gravity = FVector(0.0f, -9.8f, 0.0f);
	float MaxParticles = 1000.0f;
};

// 설명 : GPU 기반 파티클 렌더러 컴포넌트
//        지오메트리 셰이더를 사용하여 포인트를 빌보드로 확장하여 렌더링
class UParticleRenderer : public URenderer
{
public:
	ENGINEAPI UParticleRenderer();
	ENGINEAPI ~UParticleRenderer();

	// 파티클 초기화
	ENGINEAPI void Initialize(UINT _MaxParticles);

	// 파티클 추가
	ENGINEAPI void AddParticle(const FVector& _Position, const FVector& _Velocity, const FVector2D& _Size,
		const FVector& _Color, float _Alpha, float _LifeTime);

	// 파티클 업데이트 (CPU)
	ENGINEAPI void UpdateParticles(float _DeltaTime);

	// 렌더링
	ENGINEAPI void Render(UEngineCamera* _Camera, float _DeltaTime) override;

	// 셰이더 설정
	ENGINEAPI void SetParticleShader(std::string_view _VSName, std::string_view _GSName, std::string_view _PSName);
	ENGINEAPI void SetParticleTexture(std::string_view _TextureName);

	// 중력 설정
	void SetGravity(const FVector& _Gravity)
	{
		SystemData.Gravity = _Gravity;
	}

	// 파티클 개수 반환
	UINT GetActiveParticleCount() const
	{
		return static_cast<UINT>(Particles.size());
	}

	// 모든 파티클 제거
	void ClearParticles()
	{
		Particles.clear();
		Velocities.clear();
	}

protected:
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void ComponentTick(float _DeltaTime) override;

private:
	// GPU 버퍼 생성
	void CreateParticleBuffer();
	void UpdateParticleBuffer();

	// 파티클 데이터
	std::vector<FGPUParticle> Particles;
	std::vector<FVector> Velocities; // 속도 (CPU에서 관리)

	UINT MaxParticles = 10000;
	FParticleSystemData SystemData;

	// DirectX 리소스
	Microsoft::WRL::ComPtr<ID3D11Buffer> ParticleVertexBuffer = nullptr;

	// 셰이더 리소스
	std::string VertexShaderName = "ParticleVS";
	std::string GeometryShaderName = "ParticleGS";
	std::string PixelShaderName = "ParticlePS";
	std::string TextureName = "";

	bool IsInitialized = false;

private:
	// delete Function
	UParticleRenderer(const UParticleRenderer& _Other) = delete;
	UParticleRenderer(UParticleRenderer&& _Other) noexcept = delete;
	UParticleRenderer& operator=(const UParticleRenderer& _Other) = delete;
	UParticleRenderer& operator=(UParticleRenderer&& _Other) noexcept = delete;
};
