#pragma once
#include <string>

// Ό³Έν :
class UContentsResource
{
public:
	UContentsResource();
	virtual ~UContentsResource() = 0;

	static void LoadTitleResource();
	static void LoadPlayResource();
	static void LoadShaderResource();

protected:

private:
	static void LoadPlayResourceDirectory();
	static void LoadFont();
	static void LoadFont(const std::string& _FontName, const std::string&_Exe);

	static void LoadImageDirectory(std::string_view _DirectoryName, bool _bIsRecursive = true);
	static void LoadSoundDirectory(std::string_view _DirectoryName, bool _bIsRecursive = true);

	static void LoadImageDirectoryByThread(std::string_view _DirectoryName, bool _bIsRecursive = true);
	static void LoadSoundDirectoryByThread(std::string_view _DirectoryName, bool _bIsRecursive = true);

	static void LoadContentsResource(std::string_view _Path);
	static void LoadSpriteDirectory();

	static void LoadSpriteMetaData();

private:
	// delete Function
	UContentsResource(const UContentsResource& _Other) = delete;
	UContentsResource(UContentsResource&& _Other) noexcept = delete;
	UContentsResource& operator=(const UContentsResource& _Other) = delete;
	UContentsResource& operator=(UContentsResource&& _Other) noexcept = delete;
};

