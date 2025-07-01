#pragma once
#include <EngineBase/Object.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// ���� : ��� ����, �ؽ�ó, ����, �޽� ��� ��� ���ҽ� ���/����
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
		return std::dynamic_pointer_cast<ResType>(Find(Info.name(), _Name.data())); // �θ�Ÿ�� �����͸� �ڽ�Ÿ��(ResType)���� ��ȯ
		// ���н� std::shared_ptr�� nullptr�� ��ȯ
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
	ENGINEAPI static void PushRes(std::shared_ptr<UEngineResourceBase> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		PushRes(_Res, Info.name(), _Name, _Path);
	}

	// resources ���� �Լ����� ���� �� core������ ���
	ENGINEAPI static void PushRes(std::shared_ptr<UEngineResourceBase> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);

	ENGINEAPI UEnginePath GetPath()
	{
		return Path;
	}

protected:
	UEnginePath Path;

private:
	//                               �ؽ�ó                aaaa.png       ������
	//                               ���ؽ�����             Box             ������
	ENGINEAPI static inline std::map<std::string, std::map<std::string, std::shared_ptr<UEngineResourceBase>>> AllResources;

private:
	// delete Function
	UEngineResourceBase(const UEngineResourceBase& _Other) = delete;
	UEngineResourceBase(UEngineResourceBase&& _Other) noexcept = delete;
	UEngineResourceBase& operator=(const UEngineResourceBase& _Other) = delete;
	UEngineResourceBase& operator=(UEngineResourceBase&& _Other) noexcept = delete;
};


