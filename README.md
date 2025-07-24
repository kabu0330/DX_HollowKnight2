# 할로우 나이트(Hollow Knight) 모작
DirectX 11을 활용해 할로우 나이트를 모작했습니다.

## 플레이 영상
![bandicam 2025-07-24 22-01-29-660 (2)](https://github.com/user-attachments/assets/87d7fa07-5edc-411f-b759-326396ab8e34)  
영상 링크 : https://youtu.be/vi6KnUeedrs?si=0_Tf7Nd8dymysKVo

#### 구현
- 플레이어 (나이트)
- 플레이어 스킬 4종 (공격, 대시, 파이어볼, 집중)
- 몬스터 9종 (보스 : 실패한 기사)
- 맵 (흙의 마을 ~ 잊혀진 교차로 일부)
- 이펙트
- UI
- 타이틀 화면

![Imgae](https://github.com/user-attachments/assets/df114776-bbad-48d3-8368-164ff468c500)


### 실행방법
<details>
  1. "App" 프로젝트 우클릭, 시작 프로젝트로 설정합니다. </p>
  <img width="423" height="293" alt="image" src="https://github.com/user-attachments/assets/ae2e5c8f-06fa-4e86-a536-2beabc4cd009" /> </p>
  <img width="472" height="684" alt="image" src="https://github.com/user-attachments/assets/1df22ba6-f9f1-43a5-a19a-366b458553e6" /> </p>
2. 프로젝트 구성을 "Release"로 설정합니다. </p>
<img width="1070" height="131" alt="image" src="https://github.com/user-attachments/assets/4fbef2d8-6a33-4d73-85ac-f2d173455b45" /> </p>
3. 실행(F5)합니다.

</details>

### 조작
#### 타이틀
- 플레이 시작 : Space Bar (게임 타이틀 등장 이후 동작)


#### 플레이
- 이동 :  →, ←, ↑, ↓
- 점프 : Z (누르는 시간에 따라 점프 높이 조절)
- 대시 : C
- 공격 : X
- 파이어볼 : A 
- 집중 : A (Press)


#### 치트키
<details> 
- Hp + 1 : B</p>
- Hp - 1 : V</p>
- Mp Max : S</p>
- 다시 시작 : F4</p>
- 프리 카메라 : F8</p>
- 프리 카메라 이동 : W, A, S, D</p>
- 프리 카메라 회전 : 마우스 우클릭 누른 상태로 이동 버튼 입력</p>
</details>

___
### 핵심 엔진 프레임워크  
게임을 실행하면, 엔진이 루프를 돌기 전에 기본적인 설정을 먼저 합니다.  
* dll 로드, 스레드 풀 준비, 렌더링 파이프라인 설정, 창 크기 설정, 백버퍼 설정, 사운드 설정 등  
이후 Engine Frame() 함수를 호출하며, 엔진의 핵심 기능들이 해당 함수에서 진행됩니다.

<img width="1012" height="944" alt="image" src="https://github.com/user-attachments/assets/a0518d16-abb5-4439-a008-aeac401fe993" />  
EngineCore는 Level을 관리하는 핵심 클래스입니다.  </p>
Windows Message Loop에 편승해서 메시지를 처리하지 않는 동안에는 EngineFrame()이 동작합니다. </p>

<img width="878" height="986" alt="image" src="https://github.com/user-attachments/assets/cc6b366e-7b3d-4920-908c-4db133d341c0" />  </p>

EngineFrame의 핵심은 세 가지입니다.  
1. DeltaTime을 계산하고
2. 입력 및 사운드를 처리하고
3. Level의 Tick, Render, Collision, Release를 순서대로 호출

이를 통해 레벨에 종속된 액터들의 로직이 처리되고 화면에 렌더링되며 충돌 검사가 이뤄지고 삭제가 이루어집니다.

해당 과정을 그림으로 표현하면 아래와 같습니다. </p></p>
<img width="2455" height="1401" alt="image" src="https://github.com/user-attachments/assets/1b022dc6-698d-4bf2-bd50-91a19e6da518" />
</p>

___
### 렌더링  
1. 레벨은 카메라를
2. 카메라는 렌더러를
3. 렌더러는 렌더 유닛을 관리하는 구조로
</p></p>
최종적으로 렌더링에 필요한 설정은 Render Unit을 통해 진행하고 렌더러가 어떻게 Render Unit을 설정했는지에 따라 각기 다른 렌더러를 구현할 수 있습니다.  

```
USpriteRenderer::USpriteRenderer()
{
	CreateRenderUnit();
	SetMesh("Rect");
	SetMaterial("SpriteMaterial");

	GetRenderUnit().LinkConstantBufferData("ResultColor", ColorData);
	GetRenderUnit().LinkConstantBufferData("FSpriteData", SpriteData);
	GetRenderUnit().LinkConstantBufferData("FUVValue", UVValue);


	UVValue.PlusUVValue = {0.0f, 0.0f, 0.0f, 0.0f};
	SpriteData.CuttingPos = {0.0f, 0.0f};
	SpriteData.CuttingSize = { 1.0f, 1.0f };
	SpriteData.Pivot = { 0.5f, 0.5f };

	ColorData.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}
```

또한 렌더러가 아니여도 RenderUnit만 있으면 렌더링 기능을 쉽게 추가해줄 수 있습니다. </p>
Level의 Render()에 합류하기 위해 Collision은 예외적으로 Renderer를 사용하지 않고 해당 함수를 호출함으로써 렌더링 구조에 편입합니다.
```
void UCollision::ApplyDebugRender(UEngineCamera* _Camera, float _DeltaTime)
{
	if (false == IsDebugMode)
	{
		return;
	}

	URenderUnit Unit;

	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();
	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;

	Unit.SetMesh("Rect");
	Unit.SetMaterial("CollisionDebugMaterial");

	Unit.LinkConstantBufferData("FTransform", GetTransformRef());
	Unit.LinkConstantBufferData("OutColor", DebugColor);

	Unit.Render(_Camera, _DeltaTime);
}
```


최종적으로 레벨에서 각 카메라의 모든 RTV를 합쳐 백버퍼에 그려 출력합니다. 
<img width="2284" height="1111" alt="image" src="https://github.com/user-attachments/assets/3f6cb1cd-7989-4c75-ac0d-ee142d62f1b3" />




___
### 데이터 파싱  
2D Sprite 리소스를 편리하게 사용하기 위해 Unity의 .meta 파일을 파싱하여 스프라이트를 추출했습니다.
<img width="1974" height="907" alt="image" src="https://github.com/user-attachments/assets/5d11abd9-267d-485c-be17-b58f2ef68c23" />
```
std::shared_ptr<UEngineSprite> UEngineSprite::CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt)
{
	std::shared_ptr<UEngineTexture> Tex = UEngineTexture::Find<UEngineTexture>(_Name);

	if (nullptr == Tex)
	{
		std::string Name = _Name.data();
		std::string Ext = _DataFileExt.data();
		MSGASSERT(Name + Ext + " 의 Unity 메타 파일을 찾지 못했습니다. 파일 이름 또는 확장자 명을 확인해주세요.");
		return nullptr;
	}

	std::shared_ptr<UEngineSprite> NewSpirte = std::make_shared<UEngineSprite>();
	PushResource<UEngineSprite>(NewSpirte, _Name, "");

	UEnginePath Path = Tex->GetPath();
	std::string FileName = Path.GetFileNameToString();
	FileName += _DataFileExt;
	Path.MoveParentPath();
	Path.Append(FileName);

	UEngineFile File = Path;

	File.FileOpen("rt");

	std::string Text = File.GetAllFileText();

	std::vector<std::string> SpriteDataTexts;

	size_t StartPosition = 0;
	while (true)
	{
		size_t RectIndex = Text.find("rect:", StartPosition);
		size_t AligIndex = Text.find("outline:", RectIndex);

		if (RectIndex == std::string::npos || AligIndex == std::string::npos)
		{
			break;
		}

		NewSpirte->SpriteTexture.push_back(Tex.get());
		SpriteDataTexts.push_back(Text.substr(RectIndex, AligIndex - RectIndex));
		StartPosition = AligIndex;
	}

	FVector TexSize = Tex->GetTextureSize();

	std::vector<FSpriteData> TestData;

	for (size_t i = 0; i < SpriteDataTexts.size(); i++)
	{
		std::string Text = SpriteDataTexts[i];

		size_t Start = 0;

		FSpriteData SpriteData;

		{
			std::string Number = UEngineString::InterString(Text, "x:", "\n", Start);
			SpriteData.CuttingPos.X = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "y:", "\n", Start);
			SpriteData.CuttingPos.Y = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "width:", "\n", Start);
			SpriteData.CuttingSize.X = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "height:", "\n", Start);
			SpriteData.CuttingSize.Y = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "x:", ",", Start);
			SpriteData.Pivot.X = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "y:", "}", Start);
			SpriteData.Pivot.Y = static_cast<float>(atof(Number.c_str()));
		}

		SpriteData.CuttingPos.Y = TexSize.Y - SpriteData.CuttingPos.Y - SpriteData.CuttingSize.Y;
		SpriteData.CuttingPos.X /= TexSize.X;
		SpriteData.CuttingPos.Y /= TexSize.Y;
		SpriteData.CuttingSize.X /= TexSize.X;
		SpriteData.CuttingSize.Y /= TexSize.Y;

		TestData.push_back(SpriteData);
	}

	NewSpirte->SpriteDatas = TestData;

	return NewSpirte;
}
```
___
### 픽셀 충돌  
해당 프로젝트에서는 맵과 충돌 검사는 픽셀 충돌로 구현되었습니다.  
화면에 보여지는 png파일과 실제로 충돌 검사를 수행하는 bmp파일 간의 동기화 문제를 해결하는데 많은 고민을 했습니다.  </p>

png파일을 처리하는 것은 DX의 RenderingPipeline과 DirectTex의 힘을 빌리는 것이고  
bmp파일은 WindowsAPI의 HDC와 BITMAP을 이용하는 것으로 해당 프레임워크 구조에서는 bmp파일은 OS계층, png파일은 GameEngine 계층으로 구분됩니다.  </p>

여기서 문제는 Engine 계층은 Transform을 가지지만, 상위 계층인 OS 계층은 Transform 클래스를 포함하는 것은 계층 구조에 위배됩니다.  </p>
이 문제를 해결하기 위한 대안으로 플레이어의 데카르트 좌표계를 스크린 좌표계로 변환하여 픽셀충돌을 검사하는 방식을 채택했습니다. </p>


모든 맵은 LeftTopPos를 멤버변수로 가지고 있습니다. 해당 변수는 해당 맵의 좌상단 위치를 기억하고 있습니다.  
플레이어와 픽셀 충돌 검사를 수행할 때,   
1. 좌상단 위치를 빼서 스크린 좌표계의 원점을 기준으로
2. y축을 반전시켜 플레이어가 스크린 좌표계의 원점으로부터 얼만큼 떨어진 위치에 존재하는지 검사합니다.

다소 복잡하지만 해당 방식으로 기존의 계층 구조를 변형하지 않고 컨텐츠 계층에서 할 수 있는 아이디어로 문제를 해결했습니다.

<img width="1776" height="872" alt="image" src="https://github.com/user-attachments/assets/90ec3722-a151-4ab9-ac57-5a239cb520de" />

___
### 충돌

충돌의 경우, 언리얼 엔진의 콜리전 프리셋 기능을 흉내내어 구현했습니다.  
각 월드별로 충돌 그룹을 만들고, 충돌을 검사할 그룹끼리 연결지어줍니다.  
그리고 각 콜리전은 자신이 어떤 충돌 그룹인지 설정합니다. 
액터는 콜리전이 충돌했을 때 호출할 함수를 미리 바인딩합니다.

이후 레벨에서 충돌 그룹 간 충돌 검사를 진행하며 충돌이 확인되면 바인딩된 함수를 일제히 호출하는 방식으로 충돌 처리가 이루어집니다.  
```
void UCollisionManager::CreateCollisionProfile(AGameMode* _GameMode)
{
	// 나이트
	_GameMode->GetWorld()->CreateCollisionProfile("Knight");
	_GameMode->GetWorld()->CreateCollisionProfile("KnightObject");

	// 몬스터
	_GameMode->GetWorld()->CreateCollisionProfile("Monster");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterObject");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterPoint");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterDetect");
	_GameMode->GetWorld()->CreateCollisionProfile("MonsterHead");

	// 기타 오브젝트
	_GameMode->GetWorld()->CreateCollisionProfile("Barrier");
	_GameMode->GetWorld()->CreateCollisionProfile("BossSpawnTrigger");

	// 워프
	_GameMode->GetWorld()->CreateCollisionProfile("Door");
}

void UCollisionManager::LinkCollision(AGameMode* _GameMode)
{
	// 충돌체크 해야한다.
	_GameMode->GetWorld()->LinkCollisionProfile("Door", "Knight"); // 워프
	_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Door"); // 워프 키입력

	// 발판
	_GameMode->GetWorld()->LinkCollisionProfile("Barrier", "Knight"); 
	_GameMode->GetWorld()->LinkCollisionProfile("Barrier", "Monster"); 
	_GameMode->GetWorld()->LinkCollisionProfile("BossSpawnTrigger", "Knight"); 

	// 나이트 공격
	_GameMode->GetWorld()->LinkCollisionProfile("KnightObject", "Monster"); 
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterHead", "KnightObject"); 

	// 몬스터 공격
	_GameMode->GetWorld()->LinkCollisionProfile("Monster", "Knight"); 
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterObject", "Knight");
	_GameMode->GetWorld()->LinkCollisionProfile("MonsterDetect", "Knight");
}
```

액터별 충돌 설정은 아래와 같습니다.

```
void AKnight::CreateCollision()
{
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetCollisionProfileName("Knight");
	BodyCollision->SetScale3D({ 60.0f, 130.0f });
	BodyCollision->SetRelativeLocation(BodyRenderer->GetActorLocation());
}

void AKnight::SetCollisionEvent()
{
	BodyCollision->SetCollisionEnter(std::bind(&AKnight::Collide, this, std::placeholders::_1, std::placeholders::_2));

	BodyCollision->SetCollisionStay(std::bind(&AKnight::CheckEnterDoor, this, std::placeholders::_1, std::placeholders::_2));
}
```

이제 충돌이 발생하면 &AKnight::Colide()가 호출됩니다. 이 때 UCollision* 두 개가 인자로 넘어오고 This와 Other로 구분하여 자신과 상대를 처리할 수 있습니다.  
원활한 충돌 처리를 위해 UCollision은 자신을 소유한 액터가 누구인지 알 수 있는 GetActor() 함수를 가지고 있습니다. 






