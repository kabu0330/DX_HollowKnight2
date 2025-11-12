#include "PreCompile.h"
#include "ParticleEmitter.h"
#include <EngineBase/EngineRandom.h>
#include <EngineBase/EngineMath.h>

AParticleEmitter::AParticleEmitter()
{
	SetName("ParticleEmitter");

	// 파티클 렌더러 생성
	ParticleRenderer = CreateDefaultSubobject<UParticleRenderer>();
	RootComponent = ParticleRenderer;
}

AParticleEmitter::~AParticleEmitter()
{
}

void AParticleEmitter::BeginPlay()
{
	AActor::BeginPlay();
}

void AParticleEmitter::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (false == bIsPlaying || bIsPaused)
	{
		return;
	}

	ElapsedTime += _DeltaTime;

	// 지속 시간 체크
	if (false == Settings.Loop && ElapsedTime >= Settings.Duration)
	{
		Stop();
		return;
	}

	// 파티클 방출
	EmitParticles(_DeltaTime);

	// 파티클 업데이트
	if (nullptr != ParticleRenderer)
	{
		ParticleRenderer->UpdateParticles(_DeltaTime);
	}
}

void AParticleEmitter::Initialize(const FParticleEmitterSettings& _Settings)
{
	Settings = _Settings;

	if (nullptr != ParticleRenderer)
	{
		ParticleRenderer->Initialize(Settings.MaxParticles);
		ParticleRenderer->SetGravity(Settings.Gravity);
	}
}

void AParticleEmitter::Play()
{
	bIsPlaying = true;
	bIsPaused = false;
	ElapsedTime = 0.0f;
	EmitAccumulator = 0.0f;
}

void AParticleEmitter::Stop()
{
	bIsPlaying = false;
	bIsPaused = false;
	ElapsedTime = 0.0f;
	EmitAccumulator = 0.0f;
}

void AParticleEmitter::Pause()
{
	bIsPaused = true;
}

void AParticleEmitter::Clear()
{
	if (nullptr != ParticleRenderer)
	{
		ParticleRenderer->ClearParticles();
	}
}

void AParticleEmitter::SetParticleTexture(std::string_view _TextureName)
{
	if (nullptr != ParticleRenderer)
	{
		ParticleRenderer->SetParticleTexture(_TextureName);
	}
}

void AParticleEmitter::SetGravity(const FVector& _Gravity)
{
	Settings.Gravity = _Gravity;

	if (nullptr != ParticleRenderer)
	{
		ParticleRenderer->SetGravity(_Gravity);
	}
}

void AParticleEmitter::EmitParticles(float _DeltaTime)
{
	if (nullptr == ParticleRenderer)
	{
		return;
	}

	switch (Settings.EmitType)
	{
	case EParticleEmitType::Burst:
	{
		// 최초 한 번만 모든 파티클 방출
		if (ElapsedTime <= _DeltaTime)
		{
			for (UINT i = 0; i < Settings.MaxParticles; ++i)
			{
				FVector position = GetActorLocation() + GenerateRandomPosition();
				FVector velocity = GenerateRandomVelocity();
				FVector2D size = RandomVector2D(Settings.SizeMin, Settings.SizeMax);
				FVector color = RandomVector(Settings.ColorMin, Settings.ColorMax);
				float alpha = RandomRange(Settings.AlphaMin, Settings.AlphaMax);
				float lifeTime = RandomRange(Settings.LifeTimeMin, Settings.LifeTimeMax);

				ParticleRenderer->AddParticle(position, velocity, size, color, alpha, lifeTime);
			}
		}
	}
	break;

	case EParticleEmitType::Continuous:
	{
		// 지속적으로 파티클 방출
		EmitAccumulator += _DeltaTime;

		float emitInterval = 1.0f / Settings.EmitRate;
		int particlesToEmit = static_cast<int>(EmitAccumulator / emitInterval);

		if (particlesToEmit > 0)
		{
			EmitAccumulator -= particlesToEmit * emitInterval;

			for (int i = 0; i < particlesToEmit; ++i)
			{
				if (ParticleRenderer->GetActiveParticleCount() >= Settings.MaxParticles)
				{
					break;
				}

				FVector position = GetActorLocation() + GenerateRandomPosition();
				FVector velocity = GenerateRandomVelocity();
				FVector2D size = RandomVector2D(Settings.SizeMin, Settings.SizeMax);
				FVector color = RandomVector(Settings.ColorMin, Settings.ColorMax);
				float alpha = RandomRange(Settings.AlphaMin, Settings.AlphaMax);
				float lifeTime = RandomRange(Settings.LifeTimeMin, Settings.LifeTimeMax);

				ParticleRenderer->AddParticle(position, velocity, size, color, alpha, lifeTime);
			}
		}
	}
	break;

	case EParticleEmitType::Custom:
		// 사용자가 직접 AddParticle 호출
		break;
	}
}

FVector AParticleEmitter::GenerateRandomVelocity()
{
	float speed = RandomRange(Settings.SpeedMin, Settings.SpeedMax);

	FVector direction = Settings.Direction;
	direction.Normalize();

	// 퍼짐 각도 적용
	if (Settings.SpreadAngle > 0.0f)
	{
		UEngineRandom random;
		random.SetSeed(reinterpret_cast<long long>(this) + std::chrono::high_resolution_clock::now().time_since_epoch().count());

		float angleRad = Settings.SpreadAngle * 3.14159265f / 180.0f;
		float randomAngleX = random.Randomfloat(-angleRad, angleRad);
		float randomAngleY = random.Randomfloat(-angleRad, angleRad);

		// 간단한 회전 적용 (정확한 회전은 쿼터니언 사용 권장)
		FVector offset;
		offset.X = sin(randomAngleX) * speed;
		offset.Y = sin(randomAngleY) * speed;
		offset.Z = 0.0f;

		direction += offset;
		direction.Normalize();
	}

	return direction * speed;
}

FVector AParticleEmitter::GenerateRandomPosition()
{
	FVector position = FVector::ZERO;

	UEngineRandom random;
	random.SetSeed(reinterpret_cast<long long>(this) + std::chrono::high_resolution_clock::now().time_since_epoch().count());

	switch (Settings.EmitShape)
	{
	case EParticleEmitShape::Point:
		// 점에서 방출
		break;

	case EParticleEmitShape::Sphere:
	{
		// 구 내부 랜덤 위치
		float theta = random.Randomfloat(0.0f, 3.14159265f * 2.0f);
		float phi = random.Randomfloat(0.0f, 3.14159265f);
		float r = random.Randomfloat(0.0f, Settings.EmitRadius);

		position.X = r * sin(phi) * cos(theta);
		position.Y = r * sin(phi) * sin(theta);
		position.Z = r * cos(phi);
	}
	break;

	case EParticleEmitShape::Circle:
	{
		// 원 내부 랜덤 위치
		float angle = random.Randomfloat(0.0f, 3.14159265f * 2.0f);
		float r = random.Randomfloat(0.0f, Settings.EmitRadius);

		position.X = r * cos(angle);
		position.Y = r * sin(angle);
		position.Z = 0.0f;
	}
	break;

	case EParticleEmitShape::Cone:
	{
		// 원뿔 형태 (현재는 원과 동일, 추가 구현 필요)
		float angle = random.Randomfloat(0.0f, 3.14159265f * 2.0f);
		float r = random.Randomfloat(0.0f, Settings.EmitRadius);

		position.X = r * cos(angle);
		position.Y = r * sin(angle);
		position.Z = 0.0f;
	}
	break;

	case EParticleEmitShape::Box:
	{
		// 박스 내부 랜덤 위치
		position.X = random.Randomfloat(-Settings.EmitBoxSize.X * 0.5f, Settings.EmitBoxSize.X * 0.5f);
		position.Y = random.Randomfloat(-Settings.EmitBoxSize.Y * 0.5f, Settings.EmitBoxSize.Y * 0.5f);
		position.Z = random.Randomfloat(-Settings.EmitBoxSize.Z * 0.5f, Settings.EmitBoxSize.Z * 0.5f);
	}
	break;

	case EParticleEmitShape::Direction:
		// 방향으로 방출 (위치는 기본 점)
		break;
	}

	return position;
}

float AParticleEmitter::RandomRange(float _Min, float _Max)
{
	UEngineRandom random;
	random.SetSeed(reinterpret_cast<long long>(this) + std::chrono::high_resolution_clock::now().time_since_epoch().count());
	return random.Randomfloat(_Min, _Max);
}

FVector AParticleEmitter::RandomVector(const FVector& _Min, const FVector& _Max)
{
	return FVector(
		RandomRange(_Min.X, _Max.X),
		RandomRange(_Min.Y, _Max.Y),
		RandomRange(_Min.Z, _Max.Z)
	);
}

FVector2D AParticleEmitter::RandomVector2D(const FVector2D& _Min, const FVector2D& _Max)
{
	return FVector2D(
		RandomRange(_Min.X, _Max.X),
		RandomRange(_Min.Y, _Max.Y)
	);
}
