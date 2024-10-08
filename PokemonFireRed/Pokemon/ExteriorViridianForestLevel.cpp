﻿#include "ExteriorViridianForestLevel.h"
#include "Trainer.h"
#include "ItemBall.h"
#include "EventMacros.h"
#include "EventStream.h"

UExteriorViridianForestLevel::UExteriorViridianForestLevel()
{
}

UExteriorViridianForestLevel::~UExteriorViridianForestLevel()
{
}

void UExteriorViridianForestLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	AreaBgm = RN::BgmViridianForest;

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::PlayerCharacter, { 10, 20 });				// 회색시티 앞
	//UEventManager::SetPoint(GetName(), Global::Player, { 35, 62 });			// 상록시티 앞

	MakeBugCatcher0();
	MakeBugCatcher1();
	MakeExitToR2();
	MakeExitsToPewter();
	MakeAntidoteBall();
}

void UExteriorViridianForestLevel::MakeBugCatcher0()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::BugCatcher0);
	Setting.SetPoint({ 53, 45 });
	Setting.SetDirection(FTileVector::Left);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageName(RN::BugCatcher);
	Setting.SetAnimNamePrefix("BugCatcher");

	BugCatcher0 = SpawnEventTrigger<ATrainer>(Setting);
	BugCatcher0->SetBattler("BUG CATCHER RICK", RN::BugCatcherBattler);
	BugCatcher0->SetPlayerWinMessage({ L"No!\nCATERPIE can't hack it!" });
	BugCatcher0->AddPokemonToEntry(UPokemon(EPokemonId::Weedle, 6));
	BugCatcher0->AddPokemonToEntry(UPokemon(EPokemonId::Caterpie, 6));

	AEventTrigger* WinTrigger = SpawnEventTrigger<AEventTrigger>("BugCatcher0WinTrigger");
	WinTrigger->RegisterEvent(
		EEventTriggerAction::Direct,
		SKIP_CHECK,
		ES::Start(false)
		>> ES::Achieve(EAchievement::FightWithBugCatcher0)
		>> ES::FadeOutBgm(0.5f)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(Global::ExteriorViridianForestLevel)
		>> ES::PlayBgm(RN::BgmViridianForest)
		>> ES::FadeInBgm(0.5f)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	BugCatcher0->SetAfterBattleTrigger(WinTrigger);

	for (int i = 0; i < 6; ++i)
	{
		UEventTargetSetting BattleTileSetting;
		BattleTileSetting.SetName("BugCatcher0BattleTile" + std::to_string(i));
		BattleTileSetting.SetPoint({ 45 + i, 45 });
		
		AEventTrigger* BattleTile = SpawnEventTrigger<AEventTrigger>(BattleTileSetting);

		BattleTile->RegisterEvent(
			EEventTriggerAction::StepOn,
			FightWithBugCatcher0Checker,
			ES::Start(true)
			>> ES::PlayBgm(RN::BgmEncounterBoy)
			>> ES::Surprise(EN::BugCatcher0)
			>> ES::MoveDynamicPath(EN::BugCatcher0, BugCatcher0PathGenerator, Global::CharacterWalkSpeed, false)
			>> ES::Chat({ L"Hey! you have POKéMON!\nCome on!\nLet's battle 'em!" }, EFontColor::Blue, 16)
			>> ES::PlayBgm(RN::BgmTrainerBattle)
			>> ES::Wait(0.25f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::FadeIn(0.15f)
			>> ES::Wait(0.15f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::FadeIn(0.15f)
			>> ES::Wait(0.15f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::TrainerBattle(BugCatcher0)
			>> ES::FadeIn(0.5f, EFadeType::VCurtain)
			>> ES::Wait(0.5f)
			>> ES::End(true)
		);
	}
}

void UExteriorViridianForestLevel::MakeBugCatcher1()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::BugCatcher1);
	Setting.SetPoint({ 13, 22 });
	Setting.SetDirection(FTileVector::Left);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageName(RN::BugCatcher);
	Setting.SetAnimNamePrefix("BugCatcher");

	BugCatcher1 = SpawnEventTrigger<ATrainer>(Setting);
	BugCatcher1->SetBattler("BUG CATCHER SAMMY", RN::BugCatcherBattler);
	BugCatcher1->SetPlayerWinMessage({ L"I give!\nYou're good at this!" });
	BugCatcher1->AddPokemonToEntry(UPokemon(EPokemonId::Weedle, 10));

	AEventTrigger* WinTrigger = SpawnEventTrigger<AEventTrigger>("BugCatcher1WinTrigger");
	WinTrigger->RegisterEvent(
		EEventTriggerAction::Direct,
		SKIP_CHECK,
		ES::Start(false)
		>> ES::Achieve(EAchievement::FightWithBugCatcher1)
		>> ES::FadeOutBgm(0.5f)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(Global::ExteriorViridianForestLevel)
		>> ES::PlayBgm(RN::BgmViridianForest)
		>> ES::FadeInBgm(0.5f)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	BugCatcher1->SetAfterBattleTrigger(WinTrigger);

	for (int i = 0; i < 4; ++i)
	{
		UEventTargetSetting BattleTileSetting;
		BattleTileSetting.SetName("BugCatcher1BattleTile" + std::to_string(i));
		BattleTileSetting.SetPoint({ 9 + i, 22 });

		AEventTrigger* BattleTile = SpawnEventTrigger<AEventTrigger>(BattleTileSetting);
		BattleTile->RegisterEvent(
			EEventTriggerAction::StepOn,
			FightWithBugCatcher1Checker,
			ES::Start(true)
			>> ES::PlayBgm(RN::BgmEncounterBoy)
			>> ES::Surprise(EN::BugCatcher1)
			>> ES::MoveDynamicPath(EN::BugCatcher1, BugCatcher1PathGenerator, Global::CharacterWalkSpeed, false)
			>> ES::Chat({ L"Hey Wait up!\nWhat's the hurry? Why the rush?" }, EFontColor::Blue, 16)
			>> ES::PlayBgm(RN::BgmTrainerBattle)
			>> ES::Wait(0.25f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::FadeIn(0.15f)
			>> ES::Wait(0.15f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::FadeIn(0.15f)
			>> ES::Wait(0.15f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::TrainerBattle(BugCatcher1)
			>> ES::FadeIn(0.5f, EFadeType::VCurtain)
			>> ES::Wait(0.5f)
			>> ES::End(true)
		);
	}
}

void UExteriorViridianForestLevel::MakeExitToR2()
{
	std::function<bool()> Cond = []() {
		const APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(EN::PlayerCharacter);
		return Player->GetDirection() == FTileVector::Down;
	};

	for (int i = 0; i < 3; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ExitToR2Target" + std::to_string(i));
		Setting.SetPoint({ 34 + i, 63 });
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		Trigger->RegisterEvent(
			EEventTriggerAction::ArrowClick,
			Cond,
			ES::Start(true)
			>> ES::FadeOutBgm(0.75f)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorPalletTownLevel)
			>> ES::ChangeArea("ROUTE 2", RN::BgmRoute2)
			>> ES::ChangePoint(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, { 69, 26 })
			>> ES::ChangeDirection(Global::ExteriorPalletTownLevel, Global::PlayerCharacter, FTileVector::Down)
			>> ES::CameraFocus(Global::PlayerCharacter)
			>> ES::ShowMapName(L"ROUTE 2")
			>> ES::PlayBgm(RN::BgmRoute2)
			>> ES::FadeInBgm(0.75f)
			>> ES::FadeIn(0.75f)
			>> ES::Wait(0.75f)
			>> ES::End(true)
		);
	}
}

void UExteriorViridianForestLevel::MakeExitsToPewter()
{
	for (int i = 0; i < 3; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ExitToPewterTarget" + std::to_string(i));
		Setting.SetPoint({ 10 + i, 9 });
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		Trigger->RegisterEvent(
			EEventTriggerAction::StepOn,
			SKIP_CHECK,
			ES::Start(true)
			>> ES::FadeOutBgm(0.75f)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorPewterCityLevel)
			>> ES::ChangeArea("ROUTE 2", RN::BgmRoute2)
			>> ES::ChangePoint(Global::ExteriorPewterCityLevel, EN::PlayerCharacter, { 22, 57 })
			>> ES::ChangeDirection(Global::ExteriorPewterCityLevel, Global::PlayerCharacter, FTileVector::Up)
			>> ES::CameraFocus(Global::PlayerCharacter)
			>> ES::ShowMapName(L"ROUTE 2")
			>> ES::PlayBgm(RN::BgmRoute2)
			>> ES::FadeInBgm(0.75f)
			>> ES::FadeIn(0.75f)
			>> ES::Wait(0.75f)
			>> ES::End(true)
		);
	}
}

void UExteriorViridianForestLevel::MakeAntidoteBall()
{
	UEventTargetSetting AntidoteBallSetting;
	AntidoteBallSetting.SetName("VFAntidoteBall");
	AntidoteBallSetting.SetPoint({ 50, 43 });
	AntidoteBallSetting.SetDirection(FTileVector::Down);
	AntidoteBallSetting.SetCollidable(true);
	AntidoteBallSetting.SetImageName(RN::MapBall);
	AntidoteBallSetting.SetHeight(1);

	AItemBall* AntidoteBall = SpawnEventTrigger<AItemBall>(AntidoteBallSetting);
	AntidoteBall->SetItem(EItemId::Antidote, 5);
	AntidoteBall->RegisterPredefinedEvent();
}

bool UExteriorViridianForestLevel::FightWithBugCatcher0Checker()
{
	return false == UPlayerData::IsAchieved(EAchievement::FightWithBugCatcher0);
}

bool UExteriorViridianForestLevel::FightWithBugCatcher1Checker()
{
	return false == UPlayerData::IsAchieved(EAchievement::FightWithBugCatcher1);
}

std::vector<FTileVector> UExteriorViridianForestLevel::BugCatcher0PathGenerator()
{
	const AEventTarget* Player = UEventManager::FindCurLevelTarget<AEventTarget>(EN::PlayerCharacter);
	const AEventTarget* BugCatcher0 = UEventManager::FindCurLevelTarget<AEventTarget>(EN::BugCatcher0);

	int DiffX = BugCatcher0->GetPoint().X - Player->GetPoint().X;

	std::vector<FTileVector> Path;

	for (int i = 0; i < DiffX-1; ++i)
	{
		Path.push_back(FTileVector::Left);
	}

	return Path;
}

std::vector<FTileVector> UExteriorViridianForestLevel::BugCatcher1PathGenerator()
{
	const AEventTarget* Player = UEventManager::FindCurLevelTarget<AEventTarget>(EN::PlayerCharacter);
	const AEventTarget* BugCatcher1 = UEventManager::FindCurLevelTarget<AEventTarget>(EN::BugCatcher1);

	int DiffX = BugCatcher1->GetPoint().X - Player->GetPoint().X;

	std::vector<FTileVector> Path;

	for (int i = 0; i < DiffX-1; ++i)
	{
		Path.push_back(FTileVector::Left);
	}

	return Path;
}
