#pragma once

// Ό³Έν :
class UGameInstance
{
public:
	ENGINEAPI UGameInstance();
	ENGINEAPI virtual ~UGameInstance();


protected:

private:
	// delete Function
	UGameInstance(const UGameInstance& _Other) = delete;
	UGameInstance(UGameInstance&& _Other) noexcept = delete;
	UGameInstance& operator=(const UGameInstance& _Other) = delete;
	UGameInstance& operator=(UGameInstance&& _Other) noexcept = delete;
};

