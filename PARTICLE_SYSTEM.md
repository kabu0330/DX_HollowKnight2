# GPU 기반 파티클 시스템

## 개요

DirectX 11 지오메트리 셰이더를 활용한 현대적인 GPU 기반 파티클 시스템입니다.

### 주요 특징

- ✅ **지오메트리 셰이더 기반**: 포인트를 빌보드 쿼드로 GPU에서 확장
- ✅ **단일 드로우 콜**: 수천 개의 파티클을 한 번의 드로우 콜로 렌더링
- ✅ **동적 버퍼 관리**: GPU 정점 버퍼를 동적으로 업데이트
- ✅ **자동 빌보드**: 항상 카메라를 향하도록 자동 회전
- ✅ **생명 주기 관리**: 자동 페이드 아웃 및 파티클 제거
- ✅ **다양한 방출 모드**: Burst, Continuous, Custom
- ✅ **다양한 방출 형태**: Point, Sphere, Circle, Cone, Box, Direction

## 아키텍처

```
┌──────────────────────────────────────────┐
│         AParticleEmitter (Actor)          │  사용자 인터페이스
│  - 파티클 방출 제어                        │
│  - 설정 관리                              │
└────────────────┬─────────────────────────┘
                 │
                 │ 소유
                 ▼
┌──────────────────────────────────────────┐
│    UParticleRenderer (Component)         │  GPU 통신 계층
│  - GPU 버퍼 관리                          │
│  - 셰이더 바인딩                          │
│  - 드로우 콜 실행                         │
└────────────────┬─────────────────────────┘
                 │
                 │ 사용
                 ▼
┌──────────────────────────────────────────┐
│      EngineParticleShader.fx             │  GPU 렌더링 파이프라인
│  ┌────────────────────────────────────┐  │
│  │ VS: 포인트 전달                     │  │
│  └─────────────┬──────────────────────┘  │
│                ▼                          │
│  ┌────────────────────────────────────┐  │
│  │ GS: 포인트 → 빌보드 쿼드 확장      │  │  ⭐ 핵심!
│  │     (4개 정점 생성)                 │  │
│  └─────────────┬──────────────────────┘  │
│                ▼                          │
│  ┌────────────────────────────────────┐  │
│  │ PS: 텍스처 샘플링 & 블렌딩         │  │
│  └────────────────────────────────────┘  │
└──────────────────────────────────────────┘
```

## 기존 시스템과의 비교

| 항목 | 기존 시스템 (AParticle) | 새 시스템 (AParticleEmitter) |
|------|------------------------|------------------------------|
| 렌더링 방식 | 각 파티클마다 개별 액터 생성 | 단일 드로우 콜 |
| 드로우 콜 | 파티클 개수만큼 | **1회** |
| GPU 활용 | ❌ CPU 기반 | ✅ GPU 기반 (지오메트리 셰이더) |
| 성능 | 낮음 (수백 개 제한) | **높음 (수만 개 가능)** |
| 메모리 | 높음 (액터 오버헤드) | 낮음 (GPU 버퍼) |

### 성능 차이

```
기존 시스템: 1000개 파티클 = 1000 드로우 콜 ❌
새 시스템:   10000개 파티클 = 1 드로우 콜   ✅
```

## 사용 방법

### 1. 기본 사용법

```cpp
// PlayGameMode.cpp
void APlayGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 파티클 이미터 생성
    std::shared_ptr<AParticleEmitter> Emitter = GetWorld()->SpawnActor<AParticleEmitter>();

    // 설정 구성
    FParticleEmitterSettings settings;
    settings.EmitType = EParticleEmitType::Continuous;
    settings.EmitShape = EParticleEmitShape::Sphere;
    settings.MaxParticles = 5000;
    settings.EmitRate = 100.0f;           // 초당 100개
    settings.Duration = 5.0f;             // 5초간 방출
    settings.Loop = true;                 // 반복

    // 파티클 속성
    settings.LifeTimeMin = 1.0f;
    settings.LifeTimeMax = 3.0f;
    settings.SizeMin = FVector2D(0.5f, 0.5f);
    settings.SizeMax = FVector2D(1.5f, 1.5f);
    settings.ColorMin = FVector(1.0f, 0.5f, 0.0f);  // 주황색
    settings.ColorMax = FVector(1.0f, 1.0f, 0.0f);  // 노란색
    settings.AlphaMin = 0.8f;
    settings.AlphaMax = 1.0f;

    // 속도 설정
    settings.SpeedMin = 2.0f;
    settings.SpeedMax = 5.0f;
    settings.Direction = FVector::UP;
    settings.SpreadAngle = 45.0f;         // 45도 퍼짐

    // 물리 설정
    settings.Gravity = FVector(0.0f, -9.8f, 0.0f);

    // 초기화 및 재생
    Emitter->Initialize(settings);
    Emitter->SetParticleTexture("ParticleTexture");
    Emitter->SetEmitPosition(FVector(0.0f, 100.0f, 0.0f));
    Emitter->Play();
}
```

### 2. 폭발 효과 (Burst)

```cpp
void CreateExplosion(const FVector& _Position)
{
    std::shared_ptr<AParticleEmitter> Explosion = GetWorld()->SpawnActor<AParticleEmitter>();

    FParticleEmitterSettings settings;
    settings.EmitType = EParticleEmitType::Burst;  // 한 번에 모두 방출
    settings.EmitShape = EParticleEmitShape::Sphere;
    settings.MaxParticles = 1000;

    settings.LifeTimeMin = 0.5f;
    settings.LifeTimeMax = 1.5f;
    settings.SpeedMin = 5.0f;
    settings.SpeedMax = 15.0f;

    settings.ColorMin = FVector(1.0f, 0.3f, 0.0f);  // 붉은색
    settings.ColorMax = FVector(1.0f, 0.8f, 0.0f);  // 주황색

    Explosion->Initialize(settings);
    Explosion->SetEmitPosition(_Position);
    Explosion->Play();
}
```

### 3. 불 효과 (Fire)

```cpp
void CreateFire(const FVector& _Position)
{
    std::shared_ptr<AParticleEmitter> Fire = GetWorld()->SpawnActor<AParticleEmitter>();

    FParticleEmitterSettings settings;
    settings.EmitType = EParticleEmitType::Continuous;
    settings.EmitShape = EParticleEmitShape::Circle;
    settings.MaxParticles = 2000;
    settings.EmitRate = 200.0f;
    settings.Loop = true;

    settings.EmitRadius = 0.5f;           // 작은 원에서 방출

    settings.LifeTimeMin = 0.8f;
    settings.LifeTimeMax = 1.5f;
    settings.SizeMin = FVector2D(0.3f, 0.5f);
    settings.SizeMax = FVector2D(0.8f, 1.2f);

    settings.ColorMin = FVector(1.0f, 0.2f, 0.0f);  // 붉은 불
    settings.ColorMax = FVector(1.0f, 1.0f, 0.0f);  // 노란 불

    settings.SpeedMin = 1.0f;
    settings.SpeedMax = 3.0f;
    settings.Direction = FVector::UP;     // 위로 상승
    settings.SpreadAngle = 20.0f;

    settings.Gravity = FVector(0.0f, -2.0f, 0.0f);  // 약한 중력

    Fire->Initialize(settings);
    Fire->SetEmitPosition(_Position);
    Fire->Play();
}
```

### 4. 눈/비 효과

```cpp
void CreateSnow()
{
    std::shared_ptr<AParticleEmitter> Snow = GetWorld()->SpawnActor<AParticleEmitter>();

    FParticleEmitterSettings settings;
    settings.EmitType = EParticleEmitType::Continuous;
    settings.EmitShape = EParticleEmitShape::Box;   // 박스 영역에서 방출
    settings.MaxParticles = 10000;                  // 대량의 파티클
    settings.EmitRate = 500.0f;
    settings.Loop = true;

    settings.EmitBoxSize = FVector(50.0f, 1.0f, 50.0f);  // 넓은 영역

    settings.LifeTimeMin = 3.0f;
    settings.LifeTimeMax = 5.0f;
    settings.SizeMin = FVector2D(0.1f, 0.1f);
    settings.SizeMax = FVector2D(0.3f, 0.3f);

    settings.ColorMin = FVector(1.0f, 1.0f, 1.0f);  // 흰색
    settings.ColorMax = FVector(1.0f, 1.0f, 1.0f);

    settings.SpeedMin = 0.5f;
    settings.SpeedMax = 2.0f;
    settings.Direction = FVector(0.0f, -1.0f, 0.0f);  // 아래로 떨어짐
    settings.SpreadAngle = 10.0f;

    settings.Gravity = FVector(0.0f, -1.0f, 0.0f);

    Snow->Initialize(settings);
    Snow->SetEmitPosition(FVector(0.0f, 100.0f, 0.0f));
    Snow->Play();
}
```

## 셰이더 로딩

```cpp
// ContentsCore.cpp 또는 리소스 초기화 부분
void LoadParticleShaders()
{
    // 파티클 셰이더 로드
    UEngineVertexShader::LoadVertexShader("ParticleVS",
        "EngineShader\\EngineParticleShader.fx", "ParticleVS");

    UEngineGeometryShader::LoadGeometryShader("ParticleGS",
        "EngineShader\\EngineParticleShader.fx", "ParticleGS");

    UEnginePixelShader::LoadPixelShader("ParticlePS",
        "EngineShader\\EngineParticleShader.fx", "ParticlePS");
}
```

## API 레퍼런스

### AParticleEmitter

#### 주요 메서드

- `void Initialize(const FParticleEmitterSettings& _Settings)` - 이미터 초기화
- `void Play()` - 파티클 방출 시작
- `void Stop()` - 파티클 방출 정지
- `void Pause()` - 일시 정지
- `void Clear()` - 모든 파티클 제거
- `void SetParticleTexture(std::string_view _TextureName)` - 텍스처 설정
- `void SetGravity(const FVector& _Gravity)` - 중력 설정

### FParticleEmitterSettings

#### 방출 설정
- `EParticleEmitType EmitType` - 방출 타입 (Burst, Continuous, Custom)
- `EParticleEmitShape EmitShape` - 방출 형태 (Point, Sphere, Circle, Cone, Box)
- `UINT MaxParticles` - 최대 파티클 개수
- `float EmitRate` - 초당 방출 개수
- `float Duration` - 방출 지속 시간
- `bool Loop` - 반복 여부

#### 파티클 속성
- `float LifeTimeMin/Max` - 생명 시간 범위
- `FVector2D SizeMin/Max` - 크기 범위
- `FVector ColorMin/Max` - 색상 범위
- `float AlphaMin/Max` - 알파 범위

#### 물리 설정
- `float SpeedMin/Max` - 속도 범위
- `FVector Direction` - 방출 방향
- `float SpreadAngle` - 퍼짐 각도
- `FVector Gravity` - 중력

## 성능 최적화 팁

1. **적절한 파티클 개수 설정**
   - 대부분의 효과: 1000~5000개
   - 배경 효과 (눈, 비): 10000~20000개

2. **생명 주기 최소화**
   - 짧은 효과는 1~2초 권장
   - 메모리와 성능 향상

3. **텍스처 최적화**
   - 작은 텍스처 사용 (64x64, 128x128)
   - 알파 채널 활용

4. **블렌딩 모드**
   - 반투명 효과는 성능 비용이 있음
   - 가능하면 Additive 블렌딩 사용

## 향후 개선 사항

- [ ] 컴퓨트 셰이더를 이용한 GPU 업데이트
- [ ] 파티클 풀링 시스템
- [ ] 파티클 충돌 처리
- [ ] 텍스처 애니메이션
- [ ] 색상 그라디언트 커브
- [ ] 크기 변화 커브
- [ ] 에디터 GUI

## 참고 자료

- DirectX 11 Geometry Shader: https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-geometry-shader-stage
- GPU Particle Systems: https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-23-high-speed-screen-particles
