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
	if (Particles.size() < MaxParticles)
	{
		// ·£´ý À§Ä¡
		UEngineRandom Random;
		float RandomSpeedX = Random.Randomfloat(-100.0f, 100.0f);

		Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr)); // ÇÑ¹ø ´õ ¼ÅÇÃ
		float RandomSpeedY = Random.Randomfloat(-100.0f, 100.0f);

		FVector RandomVelocity = FVector(RandomSpeedX, RandomSpeedY);

		std::shared_ptr<AEffect> Effect = GetWorld()->SpawnActor<AEffect>();
		Effect->ChangeAnimation(EffectName);
		Effect->SetLocation(EmitterPosition);
		Effect->SetZSort(EZOrder::HitParticleEffect);
		Effect->SetScale(1.5f);
		Effect->GetRenderer()->SetAlpha(0.9f);

		FParticleData NewParticle(Effect, RandomVelocity, LifeTime);

		Particles.push_back(NewParticle);
	}
}

void AParticle::Update(float _DeltaTime)
{
	for (auto& Particle : Particles)
	{
		Particle.Position += Particle.Velocity * _DeltaTime;
		Particle.LifeTime -= _DeltaTime;

		if (nullptr != Particle.Effect && true == Particle.Effect->bIsValid)
		{
			Particle.Effect->AddActorLocation(Particle.Position);
		}
	}
}

void AParticle::Remove()
{
	Particles.erase(
		std::remove_if(Particles.begin(), Particles.end(), [](const FParticleData& _Particle)
			{
				return _Particle.LifeTime <= 0.0f;
			}),
		Particles.end());

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

