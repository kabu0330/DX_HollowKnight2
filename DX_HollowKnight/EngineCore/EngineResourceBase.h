#pragma once
#include <EngineBase/Object.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// 설명 : 상수 버퍼, 텍스처, 사운드, 메시 등등 모든 리소스 등록/관리
class UEngineResourceBase : public UObject
{
public:
	UEngineResourceBase() {}
	~UEngineResourceBase()	{}

	static std::string ToUpperName(std::string_view _Name)
	{
		return UEngineString::ToUpper(_Name);
	}

	template<typename ResType>
	ENGINEAPI static std::shared_ptr<ResType> Find(std::string_view _Name)
	{
		const type_info& Info = typeid(ResType);
		return std::dynamic_pointer_cast<ResType>(Find(Info.name(), _Name.data())); // 부모타입 포인터를 자식타입(ResType)으로 변환
		// 실패시 std::shared_ptr을 nullptr로 반환
	}

	ENGINEAPI static std::shared_ptr<UEngineResourceBase> Find(std::string_view _ResName, std::string_view _Name);

	static bool Contains(std::string_view _Name)
	{
		return AllResources.contains(_Name.data());
	}

	static void Release()
	{
		AllResources.clear();
	}

	template<typename ResType>
	ENGINEAPI static void PushResourceThreadSafe(std::shared_ptr<UEngineResourceBase> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		PushResourceThreadSafe(_Res, Info.name(), _Name, _Path);
	}

	ENGINEAPI static void PushResourceThreadSafe(std::shared_ptr<UEngineResourceBase> _Res, std::string_view _Info, std::string_view _Name, std::string_view _Path);

	template<typename ResType>
	ENGINEAPI static void PushResource(std::shared_ptr<UEngineResourceBase> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		PushResource(_Res, Info.name(), _Name, _Path);
	}

	// resources 관리 함수들은 전부 다 core에서만 사용
	ENGINEAPI static void PushResource(std::shared_ptr<UEngineResourceBase> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);

	ENGINEAPI UEnginePath GetPath()
	{
		return Path;
	}

protected:
	UEnginePath Path;

private:
	//                               텍스처                aaaa.png       데이터
	//                               버텍스버퍼             Box             데이터
	ENGINEAPI static inline std::map<std::string, std::map<std::string, std::shared_ptr<UEngineResourceBase>>> AllResources;

private:
	// delete Function
	UEngineResourceBase(const UEngineResourceBase& _Other) = delete;
	UEngineResourceBase(UEngineResourceBase&& _Other) noexcept = delete;
	UEngineResourceBase& operator=(const UEngineResourceBase& _Other) = delete;
	UEngineResourceBase& operator=(UEngineResourceBase&& _Other) noexcept = delete;
};


