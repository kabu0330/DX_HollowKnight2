#include "PreCompile.h"
#include "EngineResourceBase.h"
#include "EngineTexture.h"
#include <mutex>

std::mutex ResCriticalSection;

std::shared_ptr<UEngineResourceBase> UEngineResourceBase::Find(std::string_view _ResName, std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == AllResources[_ResName.data()].contains(UpperString))
	{
		return nullptr;
	}

	return AllResources[_ResName.data()][UpperString];
}

void UEngineResourceBase::PushResourceThreadSafe(std::shared_ptr<UEngineResourceBase> _Res, std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	_Res->SetName(UpperName);
	_Res->Path = _Path;

	{
		std::lock_guard<std::mutex> Lock(ResCriticalSection);
		
		if (true == AllResources[_Info.data()].contains(UpperName))
		{
			MSGASSERT("�̹� �ε��� ���ҽ��Դϴ�." + std::string(_Info.data()) + " " + _Name.data());
			return;
		}

		AllResources[_Info.data()].insert({ UpperName, _Res });
	}
}

//                                              �޸� ���                    �ڷ���                     ���� ���� �̸�             ���(��������)
void UEngineResourceBase::PushRes(std::shared_ptr<UEngineResourceBase> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	//       �ؽ�ó��                  Player.png�� ����ִ��� Ȯ��.
	if (true == AllResources[_Info.data()].contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���ҽ��� �� �ε� �Ϸ��� �߽��ϴ�" + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	_Res->SetName(UpperName); // �ؽ�ó �̸�
	_Res->Path = _Path; // �ؽ�ó ���
	AllResources[_Info.data()].insert({UpperName, _Res }); // �ڷ����� �������� �ٽ�, �ؽ�ó �̸��� �������� �ؽ�ó �����͸� ����

	return;
}