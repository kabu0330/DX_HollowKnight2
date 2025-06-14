#pragma once
#include <string>

// ���� :
class UContentsResource
{
public:
	UContentsResource();
	virtual ~UContentsResource() = 0;

	static void LoadContentsResource(std::string_view _Path);
	static void LoadResourceDirectory();
	static void LoadFolder();
	static void LoadSprite();
	static void LoadShaderResource();
protected:

private:
	static void LoadResource();
	static void LoadFont();

private:
	// delete Function
	UContentsResource(const UContentsResource& _Other) = delete;
	UContentsResource(UContentsResource&& _Other) noexcept = delete;
	UContentsResource& operator=(const UContentsResource& _Other) = delete;
	UContentsResource& operator=(UContentsResource&& _Other) noexcept = delete;
};

