#include "PokemonUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonUILevelManager.h"
#include "MapLevel.h"

UPokemonUILevel::UPokemonUILevel()
{
}

UPokemonUILevel::~UPokemonUILevel()
{
}

void UPokemonUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	// �⺻ ��� ����
	UEngineDirectory CurDir;
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("UIPokemonLevel");

	std::list<UEngineFile> Files = CurDir.AllFile();
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	Manager = SpawnActor<UPokemonUILevelManager>();
}

void UPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);

	if (nullptr != MapLevel)
	{
		Manager->SetPrevMapLevelName(MapLevel->GetName());
	}
}