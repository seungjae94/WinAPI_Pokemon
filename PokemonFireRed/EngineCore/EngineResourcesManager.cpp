#include "EngineResourcesManager.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

UEngineResourcesManager::UEngineResourcesManager() 
{
}

UEngineResourcesManager::~UEngineResourcesManager() 
{
}

UWindowImage* UEngineResourcesManager::LoadImg(std::string_view _Path)
{
	return nullptr;
}

UWindowImage* UEngineResourcesManager::LoadImg(std::string_view _Path, std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (Images.contains(UpperName))
	{
		MsgBoxAssert(std::string("��� : ") + std::string(_Path) + "\n���ϸ� : " + std::string(_Name) + "\n�̹� �ε��� ������ �� �ε��Ϸ��� �߽��ϴ�");
		return nullptr;
	}

	UWindowImage* NewImage = new UWindowImage();
	Images[UpperName] = NewImage;
	return NewImage;
}

UWindowImage* UEngineResourcesManager::FindImg(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (!Images.contains(UpperName))
	{
		MsgBoxAssert("���ϸ� : " + std::string(_Name) + "\n�������� �ʴ� �̹����Դϴ�");
		return nullptr;
	}

	return Images[UpperName];
}