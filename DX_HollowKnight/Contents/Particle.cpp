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
			MSGASSERT("��ƼŬ Ÿ���� REVERSE�� ���� ������ �ʼ� �Է� �Ű������Դϴ�.");
			return;
		}

		_Dir.Normalize();

		for (int i = 0; i < MaxParticles; i++)
		{
			// ���� ��ġ
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
			// ���� ��ġ
			UEngineRandom Random;
			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i);
			float RandomSpeedX = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + (i * 2)); // �ѹ� �� ����
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
		return; // �ִ� ���� ���� �ʰ�
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
				// Effect�� ��ȿ���� �ʰų� LifeTime�� ���� ��� ����
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

