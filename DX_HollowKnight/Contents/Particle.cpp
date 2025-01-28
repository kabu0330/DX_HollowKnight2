#include "PreCompile.h"
#include "Particle.h"
#include <EngineBase/EngineRandom.h>
#include "Effect.h"

AParticle::AParticle()
{
	SetName("AParticle");
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
}

AParticle::~AParticle()
{
}

void AParticle::SpawnParticle()
{
	int RemainParticles = MaxParticles - Count;

	if (0 >= RemainParticles) 
	{
		return; // 최대 생성 개수 초과
	}

	EffectInitPos = EmitterPosition;

	for (int i = 0; i < RemainParticles; i++)
	{
		// 랜덤 위치
		UEngineRandom Random;
		Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i); // 한번 더 셔플
		float RandomSpeedX = Random.Randomfloat(-200.0f, 200.0f);

		Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + (i * 2)); // 한번 더 셔플
		float RandomSpeedY = Random.Randomfloat(-200.0f, 200.0f);

		FVector RandomVelocity = FVector(RandomSpeedX, RandomSpeedY);

		std::shared_ptr<AEffect> Effect = GetWorld()->SpawnActor<AEffect>();
		Effect->ChangeAnimation(EffectName);

		FVector InitPos = EmitterPosition + RandomVelocity;

		Effect->SetLocation(InitPos);
		Effect->SetZSort(EZOrder::HitParticleEffect);
		Effect->SetScale(1.5f);
		Effect->GetRenderer()->SetAlpha(0.9f);
		Effect->SetZSort(static_cast<int>(EZOrder::HitParticleEffect) + i + 1);

		FParticleData NewParticle(Effect, RandomVelocity);

		Particles.push_back(NewParticle);
		++Count;
	}
}

void AParticle::Update(float _DeltaTime)
{
	for (auto& Particle : Particles)
	{
		Particle.Position += Particle.Velocity * _DeltaTime * 5.0f;

		if (nullptr != Particle.Effect && true == Particle.Effect->bIsValid)
		{
			Particle.Effect->SetLocation(EffectInitPos + Particle.Position);
		}
	}
}

void AParticle::Remove()
{	
	Particles.erase(
		std::remove_if(Particles.begin(), Particles.end(),
			[](const FParticleData& Particle) {
				// Effect가 유효하지 않거나 LifeTime이 끝난 경우 제거
				return Particle.Effect->GetRenderer() == nullptr || true == Particle.Effect->GetRenderer()->IsCurAnimationEnd();
			}),
		Particles.end());
	if (0 >= Particles.size())
	{
		Destroy();
	}
}

void AParticle::BeginPlay()
{
	AActor::BeginPlay();
}

void AParticle::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	SpawnParticle();
	Update(_DeltaTime);
	Remove();
}

