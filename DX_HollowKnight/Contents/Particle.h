#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/Actor.h>

struct FParticleData
{
public:
	std::shared_ptr<class AEffect> Effect = nullptr;
	FVector Position = FVector::ZERO;
	FVector Velocity = FVector::ZERO;

	FParticleData(std::shared_ptr<class AEffect> _Effect, const FVector& _Velocity)
		: Effect(_Effect), Velocity(_Velocity)
	{
	}
};

enum class EParticleType
{
	RANDOM,
	REVERSE,
	MAX,
};

// 설명 :
class AParticle : public AActor
{
public:
	// constrcuter destructer
	AParticle();
	~AParticle();

	// delete Function
	AParticle(const AParticle& _Other) = delete;
	AParticle(AParticle&& _Other) noexcept = delete;
	AParticle& operator=(const AParticle& _Other) = delete;
	AParticle& operator=(AParticle&& _Other) noexcept = delete;

	void CreateParticle(std::string_view _EffectName, int _MaxParticles, float _SpawnRate, const FVector& _EmitterPosition)
	{
		EffectName = _EffectName;
		MaxParticles = _MaxParticles;
		SpawnRate = _SpawnRate;
		EmitterPosition = _EmitterPosition;
		Velocities.resize(_MaxParticles);
	}
	void SetDirection(EParticleType _ParticleType, float _MinVelocity = -100.0f, float _MaxVelocity = 100.0f, FVector _Dir = FVector::ZERO, FVector _Force = FVector::ZERO);
	void SetZOrderOffset(float _Value)
	{
		ZOrderOffset = _Value;
	}
	void SetAlpha(float _Ratio)
	{
		Alpha = _Ratio;
	}
	void SetRandomScale(float _MinRatio, float _MaxRatio)
	{
		RandomScaleMin = _MinRatio;
		RandomScaleMax = _MaxRatio;
	}

	void SpawnParticle();
	void Update(float _DeltaTime);
	void Remove();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::list<FParticleData> Particles;
	class AEffect* Effect = nullptr;
	std::string EffectName = "";
	int MaxParticles = 0;
	float SpawnRate = 0.0f; // 생성 속도
	FVector EmitterPosition = FVector::ZERO; // 파티클 생성 위치
	FVector EffectInitPos = FVector::ZERO;
	EParticleType ParticleType = EParticleType::RANDOM;
	std::vector<FVector> Velocities;
	FVector Force = FVector::ZERO;
	float ZOrderOffset = 0.0f;
	float Alpha = 0.7f;
	float RandomScaleMin = 0.2f;
	float RandomScaleMax = 0.7f;

	int Count = 0;
};

