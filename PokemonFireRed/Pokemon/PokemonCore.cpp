#include "PokemonCore.h"
#include "TitleLevel.h"
#include "TutorialLevel.h"
#include "ExteriorPalletTownLevel.h"
#include "ExteriorViridianForestLevel.h"
#include "ExteriorPewterCityLevel.h"
#include "InteriorOaksLabLevel.h"
#include "InteriorPlayersHouse1FLevel.h"
#include "InteriorPlayersHouse2FLevel.h"
#include "InteriorRivalsHouseLevel.h"
#include "InteriorPokemonCenterLevel.h"
#include "InteriorShopLevel.h"
#include "InteriorPewterGymLevel.h"
#include "BagItemPokemonUILevel.h"
#include "BattleItemPokemonUILevel.h"
#include "BattleShiftPokemonUILevel.h"
#include "MenuPokemonUILevel.h"
#include "PokemonSummaryUILevel.h"
#include "TrainerCardUILevel.h"
#include "BattleLevel.h"
#include "MenuBagUILevel.h"
#include "BattleBagUILevel.h"
#include "Global.h"

UPokemonCore::UPokemonCore()
{
}

UPokemonCore::~UPokemonCore()
{
}

void UPokemonCore::BeginPlay()
{
	// 게임 기본 설정
	MainWindow.SetWindowTitle("Pokemon FireRed");
	MainWindow.SetWindowScale({ Global::ScreenX, Global::ScreenY });
	MainWindow.SetClearColor(Color8Bit::BlackA);
	UEngineSound::SetGlobalVolume(0.25f);

	// UI 리소스 로딩
	LoadUIResources();

	// 포켓몬 리소스 로딩
	LoadPokemonResources();

	// 사운드 리소스 로딩
	LoadSounds();

	// 디버그용 데이터
	//DebugGeneratePokemons();
	//DebugGenerateItems();
	//DebugGenerateAchievements();

	// 레벨 생성
	CreateLevel<UTitleLevel>(Global::TitleLevel);
	CreateLevel<UTutorialLevel>(Global::TutorialLevel);
	CreateLevel<UExteriorPalletTownLevel>(Global::ExteriorPalletTownLevel);
	CreateLevel<UExteriorViridianForestLevel>(Global::ExteriorViridianForestLevel);
	CreateLevel<UExteriorPewterCityLevel>(Global::ExteriorPewterCityLevel);
	CreateLevel<UInteriorOaksLabLevel>(Global::InteriorOaksLabLevel);
	CreateLevel<UInteriorPlayersHouse1FLevel>(Global::InteriorPlayersHouse1FLevel);
	CreateLevel<UInteriorPlayersHouse2FLevel>(Global::InteriorPlayersHouse2FLevel);
	CreateLevel<UInteriorRivalsHouseLevel>(Global::InteriorRivalsHouseLevel);
	CreateLevel<UInteriorPokemonCenterLevel>(Global::InteriorPokemonCenterLevel);
	CreateLevel<UInteriorShopLevel>(Global::InteriorShopLevel);
	CreateLevel<UInteriorPewterGymLevel>(Global::InteriorPewterGymLevel);
	CreateLevel<UBagItemPokemonUILevel>(Global::BagItemPokemonUILevel);
	CreateLevel<UBattleItemPokemonUILevel>(Global::BattleItemPokemonUILevel);
	CreateLevel<UBattleShiftPokemonUILevel>(Global::BattleShiftPokemonUILevel);
	CreateLevel<UMenuPokemonUILevel>(Global::MenuPokemonUILevel);
	CreateLevel<UPokemonSummaryUILevel>(Global::PokemonSummaryUILevel);
	CreateLevel<UTrainerCardUILevel>(Global::TrainerCardUILevel);
	CreateLevel<UBattleLevel>(Global::BattleLevel);
	CreateLevel<UMenuBagUILevel>(Global::MenuBagUILevel);
	CreateLevel<UBattleBagUILevel>(Global::BattleBagUILevel);

	// 시작 레벨 설정
	UEventManager::SetLevel(Global::TitleLevel);
}

void UPokemonCore::LoadUIResources()
{
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("UI");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	UEngineResourcesManager::GetInst().CuttingImage(RN::NextMsgArrow, 4, 1);

	CurDir.MoveParent();
}

void UPokemonCore::LoadPokemonResources()
{
	CurDir.Move("Pokemons");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonFront, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonFrontStat0, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonFrontStat1, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonFrontStat2, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonFrontStat3, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonBack, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonBackStat0, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonBackStat1, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonBackStat2, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonBackStat3, 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage(RN::PokemonMini, 2, 154);

	CurDir.MoveParent();
}

void UPokemonCore::LoadSounds()
{
	CurDir.Move("Bgm");

	// BGM 로드
	std::list<UEngineFile> AllBgm = CurDir.AllFile({ ".mp3", ".wav" });
	for (UEngineFile& Sound : AllBgm)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	CurDir.MoveParent();

	CurDir.Move("SE");

	// SE 로드
	std::list<UEngineFile> AllSE = CurDir.AllFile({ ".mp3", ".wav" });
	for (UEngineFile& Sound : AllSE)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	CurDir.MoveParent();

	CurDir.Move("Cry");

	// 포켓몬 울음소리 로드
	std::list<UEngineFile> AllCries = CurDir.AllFile({ ".mp3", ".wav" });
	for (UEngineFile& Sound : AllCries)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	CurDir.MoveParent();
}

void UPokemonCore::DebugGeneratePokemons()
{
	UPokemon Pokemon0 = UPokemon(EPokemonId::Bulbasaur, 10);
	UPokemon Pokemon1 = UPokemon(EPokemonId::Rattata, 1);
	UPokemon Pokemon2 = UPokemon(EPokemonId::Squirtle, 2);
	UPokemon Pokemon3 = UPokemon(EPokemonId::Charmander, 7);
	UPokemon Pokemon4 = UPokemon(EPokemonId::Pidgey, 2);
	UPokemon Pokemon5 = UPokemon(EPokemonId::Caterpie, 3);
	Pokemon0.SetCurHp(Pokemon0.GetHp() / 6);
	Pokemon1.SetCurHp(Pokemon1.GetHp() / 3);
	Pokemon0.AddAccExp(0);
	Pokemon1.AddAccExp(15);
	Pokemon2.AddAccExp(5);
	Pokemon3.AddAccExp(3);
	Pokemon4.AddAccExp(1);
	UPlayerData::AddPokemonToEntry(Pokemon0);
	UPlayerData::AddPokemonToEntry(Pokemon1);
	//UPlayerData::AddPokemonToEntry(Pokemon2);
	//UPlayerData::AddPokemonToEntry(Pokemon3);
	//UPlayerData::AddPokemonToEntry(Pokemon4);
	//UPlayerData::AddPokemonToEntry(Pokemon5);
}

void UPokemonCore::DebugGenerateItems()
{
	UPlayerData::GainItem(EItemId::Potion, 1);
	UPlayerData::GainItem(EItemId::SuperPotion, 6);
	UPlayerData::GainItem(EItemId::HyperPotion, 17);
	UPlayerData::LoseItem(EItemId::HyperPotion, 10);
	UPlayerData::GainItem(EItemId::FullHeal, 9);
	UPlayerData::GainItem(EItemId::BurnHeal, 99);
	UPlayerData::GainItem(EItemId::Antidote, 99);
	UPlayerData::GainItem(EItemId::PokeBall, 99);
}

void UPokemonCore::DebugGenerateAchievements()
{
	UPlayerData::Achieve(EAchievement::GetStarterEventStart);
	UPlayerData::Achieve(EAchievement::SelectFirstPokemon);
	UPlayerData::Achieve(EAchievement::FightWithGreen);
}
