#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/Actor.h>

struct FParticleData
{
public:
	std::shared_ptr<class AEffect> Effect = nullptr;
	FVector Position = FVector::ZERO;
	FVector Velocity = FVector::ZERO;
	float LifeTime = 0.0f;

	FParticleData(std::shared_ptr<class AEffect> _Effect, const FVector& _Velocity, float _LifeTime)
		: Effect(_Effect), Velocity(_Velocity), LifeTime(_LifeTime)
	{
	}
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

	void CreateParticle(std::string_view _EffectName, int _MaxParticles, float _SpawnRate, const FVector& _EmitterPosition, float _LifeTime)
	{
		EffectName = _EffectName;
		MaxParticles = _MaxParticles;
		SpawnRate = _SpawnRate;
		EmitterPosition = _EmitterPosition;
		LifeTime = _LifeTime;
	}

	void SpawnParticle();
	void Update(float _DeltaTime);
	void Remove();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::vector<FParticleData> Particles;
	class AEffect* Effect = nullptr;
	std::string EffectName = "";
	int MaxParticles = 0;
	float SpawnRate = 0.0f; // 생성 속도
	FVector EmitterPosition = FVector::ZERO; // 파티클 생성 위치
	float LifeTime = 0.0f;
};

