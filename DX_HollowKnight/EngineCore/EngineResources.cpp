#include "PreCompile.h"
#include "EngineResources.h"
#include "EngineTexture.h"
#include <mutex>

std::mutex ResCriticalSection;

std::shared_ptr<UEngineResources> UEngineResources::Find(std::string_view _ResName, std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == ResMap[_ResName.data()].contains(UpperString))
	{
		return nullptr;
	}

	return ResMap[_ResName.data()][UpperString];
}

void UEngineResources::PushResourceThreadSafe(std::shared_ptr<UEngineResources> _Res, std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	_Res->SetName(UpperName);
	_Res->Path = _Path;

	{
		std::lock_guard<std::mutex> Lock(ResCriticalSection);
		
		if (true == ResMap[_Info.data()].contains(UpperName))
		{
			MSGASSERT("�̹� �ε��� �ؽ�ó�Դϴ�." + std::string(_Info.data()) + " " + _Name.data());
			return;
		}

		ResMap[_Info.data()].insert({ UpperName, _Res });
	}
}

//                                              �޸� ���                    �ڷ���                     ���� ���� �̸�             ���(��������)
void UEngineResources::PushRes(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	//       �ؽ�ó��                  Player.png�� ����ִ��� Ȯ��.
	if (true == ResMap[_Info.data()].contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���ҽ��� �� �ε� �Ϸ��� �߽��ϴ�" + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	_Res->SetName(UpperName); // �ؽ�ó �̸�
	_Res->Path = _Path; // �ؽ�ó ���
	ResMap[_Info.data()].insert({UpperName, _Res }); // �ڷ����� �������� �ٽ�, �ؽ�ó �̸��� �������� �ؽ�ó �����͸� ����

	return;
}