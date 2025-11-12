#pragma once
#include <EngineCore/Actor.h>
#include "ParticleRenderer.h"

// 파티클 방출 타입
enum class EParticleEmitType
{
	Burst,      // 한 번에 모두 방출
	Continuous, // 지속적으로 방출
	Custom      // 사용자 정의
};

// 파티클 방출 모양
enum class EParticleEmitShape
{
	Point,      // 점에서 방출
	Sphere,     // 구 형태로 방출
	Circle,     // 원 형태로 방출
	Cone,       // 원뿔 형태로 방출
	Box,        // 박스 형태로 방출
	Direction   // 특정 방향으로 방출
};

// 파티클 이미터 설정
struct FParticleEmitterSettings
{
	// 방출 설정
	EParticleEmitType EmitType = EParticleEmitType::Burst;
	EParticleEmitShape EmitShape = EParticleEmitShape::Point;
	UINT MaxParticles = 1000;
	float EmitRate = 100.0f;              // 초당 방출 개수
	float Duration = 1.0f;                // 방출 지속 시간
	bool Loop = false;                    // 반복 여부

	// 파티클 속성
	float LifeTimeMin = 1.0f;
	float LifeTimeMax = 2.0f;
	FVector2D SizeMin = FVector2D(0.5f, 0.5f);
	FVector2D SizeMax = FVector2D(1.0f, 1.0f);
	FVector ColorMin = FVector(1.0f, 1.0f, 1.0f);
	FVector ColorMax = FVector(1.0f, 1.0f, 1.0f);
	float AlphaMin = 1.0f;
	float AlphaMax = 1.0f;

	// 속도 설정
	float SpeedMin = 1.0f;
	float SpeedMax = 5.0f;
	FVector Direction = FVector::UP;      // 방출 방향
	float SpreadAngle = 30.0f;            // 퍼짐 각도 (도)

	// 물리 설정
	FVector Gravity = FVector(0.0f, -9.8f, 0.0f);

	// 방출 영역 크기
	float EmitRadius = 1.0f;              // 구/원 반지름
	FVector EmitBoxSize = FVector(1.0f, 1.0f, 1.0f); // 박스 크기
};

// 설명 : GPU 기반 파티클 이미터 액터
//        사용자 친화적인 인터페이스를 제공하여 쉽게 파티클 효과 생성
class AParticleEmitter : public AActor
{
public:
	AParticleEmitter();
	~AParticleEmitter();

	// 이미터 초기화
	ENGINEAPI void Initialize(const FParticleEmitterSettings& _Settings);

	// 파티클 방출 시작
	ENGINEAPI void Play();

	// 파티클 방출 정지
	ENGINEAPI void Stop();

	// 파티클 방출 일시정지
	ENGINEAPI void Pause();

	// 모든 파티클 제거
	ENGINEAPI void Clear();

	// 설정 변경
	void SetSettings(const FParticleEmitterSettings& _Settings)
	{
		Settings = _Settings;
	}

	FParticleEmitterSettings& GetSettings()
	{
		return Settings;
	}

	// 텍스처 설정
	void SetParticleTexture(std::string_view _TextureName);

	// 방출 위치 설정
	void SetEmitPosition(const FVector& _Position)
	{
		SetActorLocation(_Position);
	}

	// 중력 설정
	void SetGravity(const FVector& _Gravity);

	// 활성 상태 확인
	bool IsPlaying() const
	{
		return bIsPlaying;
	}

	// 파티클 렌더러 가져오기
	std::shared_ptr<UParticleRenderer> GetParticleRenderer()
	{
		return ParticleRenderer;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	// 파티클 방출
	void EmitParticles(float _DeltaTime);

	// 랜덤 속도 생성
	FVector GenerateRandomVelocity();

	// 랜덤 위치 생성
	FVector GenerateRandomPosition();

	// 랜덤 값 생성 헬퍼
	float RandomRange(float _Min, float _Max);
	FVector RandomVector(const FVector& _Min, const FVector& _Max);
	FVector2D RandomVector2D(const FVector2D& _Min, const FVector2D& _Max);

	// 컴포넌트
	std::shared_ptr<UParticleRenderer> ParticleRenderer = nullptr;

	// 설정
	FParticleEmitterSettings Settings;

	// 상태
	bool bIsPlaying = false;
	bool bIsPaused = false;
	float ElapsedTime = 0.0f;
	float EmitAccumulator = 0.0f; // 방출 누적 시간

private:
	// delete Function
	AParticleEmitter(const AParticleEmitter& _Other) = delete;
	AParticleEmitter(AParticleEmitter&& _Other) noexcept = delete;
	AParticleEmitter& operator=(const AParticleEmitter& _Other) = delete;
	AParticleEmitter& operator=(AParticleEmitter&& _Other) noexcept = delete;
};
