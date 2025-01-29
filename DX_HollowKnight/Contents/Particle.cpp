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

void AParticle::SetDirection(EParticleType _ParticleType, float _MinVelocity, float _MaxVelocity, FVector _Dir, FVector _Force)
{
	Force = _Force;

	switch (_ParticleType)
	{
	case EParticleType::REVERSE:
	{
		if (FVector::ZERO == _Dir)
		{
			MSGASSERT("파티클 타입이 REVERSE일 때는 방향은 필수 입력 매개변수입니다.");
			return;
		}

		_Dir.Normalize();

		for (int i = 0; i < MaxParticles; i++)
		{
			// 랜덤 위치
			UEngineRandom Random;
			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i);
			float RandomSpeed = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Velocities[i] = _Dir * RandomSpeed;
		}
	}
		break;
	case EParticleType::RANDOM:
	case EParticleType::MAX:
	{
		for (int i = 0; i < MaxParticles; i++)
		{
			// 랜덤 위치
			UEngineRandom Random;
			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i);
			float RandomSpeedX = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + (i * 2)); // 한번 더 셔플
			float RandomSpeedY = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Velocities[i] = FVector(RandomSpeedX, RandomSpeedY);
		}
	}
		break;
	default:
		break;
	}
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
		UEngineRandom Random;
		Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i);
		float RandomScale = Random.Randomfloat(RandomScaleMin, RandomScaleMax);

		std::shared_ptr<AEffect> Effect = GetWorld()->SpawnActor<AEffect>();
		Effect->ChangeAnimation(EffectName);

		Effect->SetLocation(EmitterPosition);
		Effect->SetZSort(EZOrder::HitParticleEffect);
		Effect->SetScale(RandomScale);
		Effect->GetRenderer()->SetAlpha(Alpha);
		Effect->SetZSort(static_cast<int>(EZOrder::HitParticleEffect) -( i + 1 + ZOrderOffset));

		FParticleData NewParticle(Effect, Velocities[i]);

		Particles.push_back(NewParticle);
		++Count;
	}
}

void AParticle::Update(float _DeltaTime)
{
	for (auto& Particle : Particles)
	{
		Particle.Velocity += Force * _DeltaTime;
		Particle.Position += Particle.Velocity * _DeltaTime;

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

