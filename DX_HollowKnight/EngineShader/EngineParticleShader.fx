// 파티클 정점 구조체 (입력)
struct ParticleVertex
{
	float3 Position : POSITION;
	float2 Size : SIZE;
	float4 Color : COLOR;
	float Age : AGE;
	float LifeTime : LIFETIME;
};

// 지오메트리 셰이더 출력 구조체
struct GeometryOutput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
	float4 Color : COLOR;
	float Age : AGE;
	float LifeTime : LIFETIME;
};

// 상수 버퍼
cbuffer TransformData : register(b0)
{
	float4x4 View;
	float4x4 Projection;
	float4x4 ViewProjection;
};

cbuffer ParticleSystemData : register(b1)
{
	float3 CameraPosition;
	float DeltaTime;
	float3 Gravity;
	float MaxParticles;
};

// 텍스처 및 샘플러
Texture2D ParticleTexture : register(t0);
SamplerState ParticleSampler : register(s0);

// ==========================================
// Vertex Shader
// ==========================================
// 파티클 포인트를 그대로 전달 (지오메트리 셰이더에서 확장)
ParticleVertex ParticleVS(ParticleVertex input)
{
	return input;
}

// ==========================================
// Geometry Shader
// ==========================================
// 포인트를 빌보드 쿼드로 확장
[maxvertexcount(4)]
void ParticleGS(point ParticleVertex input[1], inout TriangleStream<GeometryOutput> outputStream)
{
	// 파티클이 살아있는지 확인
	if (input[0].Age > input[0].LifeTime)
	{
		return; // 죽은 파티클은 렌더링하지 않음
	}

	ParticleVertex particle = input[0];

	// 빌보드를 위한 카메라 방향 벡터 계산
	float3 toCamera = normalize(CameraPosition - particle.Position);
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = normalize(cross(up, toCamera));
	up = cross(toCamera, right);

	// 파티클 크기
	float halfWidth = particle.Size.x * 0.5f;
	float halfHeight = particle.Size.y * 0.5f;

	// 생명 주기에 따른 알파값 계산 (페이드 아웃)
	float lifeRatio = 1.0f - (particle.Age / particle.LifeTime);
	float alpha = particle.Color.a * lifeRatio;

	// 쿼드의 4개 정점 생성 (빌보드)
	float3 vertices[4];
	vertices[0] = particle.Position + (right * -halfWidth) + (up * -halfHeight); // 좌하
	vertices[1] = particle.Position + (right * halfWidth) + (up * -halfHeight);  // 우하
	vertices[2] = particle.Position + (right * -halfWidth) + (up * halfHeight);  // 좌상
	vertices[3] = particle.Position + (right * halfWidth) + (up * halfHeight);   // 우상

	float2 uvs[4];
	uvs[0] = float2(0.0f, 1.0f); // 좌하
	uvs[1] = float2(1.0f, 1.0f); // 우하
	uvs[2] = float2(0.0f, 0.0f); // 좌상
	uvs[3] = float2(1.0f, 0.0f); // 우상

	GeometryOutput output;

	// 4개의 정점을 출력하여 쿼드 생성
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		output.Position = mul(float4(vertices[i], 1.0f), ViewProjection);
		output.UV = uvs[i];
		output.Color = float4(particle.Color.rgb, alpha);
		output.Age = particle.Age;
		output.LifeTime = particle.LifeTime;

		outputStream.Append(output);
	}

	outputStream.RestartStrip();
}

// ==========================================
// Pixel Shader
// ==========================================
float4 ParticlePS(GeometryOutput input) : SV_Target
{
	// 텍스처 샘플링
	float4 texColor = ParticleTexture.Sample(ParticleSampler, input.UV);

	// 파티클 색상과 텍스처 색상 혼합
	float4 finalColor = texColor * input.Color;

	// 알파 테스트 (완전히 투명한 픽셀은 버림)
	if (finalColor.a < 0.01f)
	{
		discard;
	}

	return finalColor;
}

// ==========================================
// 간단한 파티클 셰이더 (빌보드 없음)
// ==========================================
struct SimpleParticleVSOutput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
	float4 Color : COLOR;
};

SimpleParticleVSOutput SimpleParticleVS(ParticleVertex input)
{
	SimpleParticleVSOutput output;

	// 월드 공간에서 뷰-프로젝션 변환
	output.Position = mul(float4(input.Position, 1.0f), ViewProjection);
	output.UV = float2(0.5f, 0.5f);

	// 생명 주기에 따른 알파값 계산
	float lifeRatio = 1.0f - (input.Age / input.LifeTime);
	output.Color = float4(input.Color.rgb, input.Color.a * lifeRatio);

	return output;
}

float4 SimpleParticlePS(SimpleParticleVSOutput input) : SV_Target
{
	return input.Color;
}
