﻿#include "ExteriorPalletTownLevel.h"
#include "Global.h"
#include "ExteriorDoor.h"
#include "ClosedDoor.h"
#include "DialogueActor.h"
#include "ItemBall.h"
#include "Text.h"
#include "PlayerData.h"
#include "Pokemon.h"
#include "SoundManager.h"
#include "EventMacros.h"
#include "EventStream.h"

UExteriorPalletTownLevel::UExteriorPalletTownLevel()
{
}

UExteriorPalletTownLevel::~UExteriorPalletTownLevel()
{
}

void UExteriorPalletTownLevel::BeginPlay()
{
	UMapLevel::BeginPlay();
	AreaName = "PALLET TOWN";
	AreaBgm = RN::BgmPalletTown;

	// (디버깅) 플레이어 시작 위치 설정
	//UEventManager::SetPoint(GetName(), Global::Player, { 74, 90 });			// 상록시티 - 1번 도로 입구
	//UEventManager::SetPoint(GetName(), Global::Player, { 82, 80 });			// 상록시티 - 포켓몬 센터 근처
	//UEventManager::SetPoint(GetName(), Global::Player, { 69, 27 });			// 상록숲 앞
	UEventManager::SetPoint(GetName(), Global::PlayerCharacter, { 77, 136 });		// 태초마을 - 1번 도로 입구 
	UEventManager::SetDirection(GetName(), Global::PlayerCharacter, FTileVector::Down);

	// 마을 생성
	MakePalletTown();
	MakeViridianCity();
	MakeRoute22();

	// 연결
	MakeRoute1ToPalletAreaChanger();
	MakePalletToRoute1AreaChanger();
	MakeRoute1ToViridianAreaChanger();
	MakeViridianToRoute1AreaChanger();	
	MakeRoute2ToViridianAreaChanger();
	MakeViridianToRoute2AreaChanger();
}

void UExteriorPalletTownLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	if (nullptr == _PrevLevel)
	{
		// 디버그용
		USoundManager::PlayBgm(RN::BgmPalletTown);
	}
}

void UExteriorPalletTownLevel::MakePalletTown()
{
	MakePTOaksLabDoor();
	MakePTPlayersHouseDoor();
	MakePTRivalsHouseDoor();
	MakePTTechMan();
	MakePTGetStarterEventTriggers();
	MakePTOak();
	MakePTItemBalls();
	MakePTAnimatedTiles();
}

void UExteriorPalletTownLevel::MakePTOaksLabDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::OaksLabDoor);
	Setting.SetPoint({ 80, 147 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName(RN::GreenDoor);
	Setting.SetHeight(1);

	AExteriorDoor* OaksLabDoor = SpawnEventTrigger<AExteriorDoor>(Setting);
	OaksLabDoor->SetTargetMapName(Global::InteriorOaksLabLevel);
	OaksLabDoor->SetTargetPoint({ 6, 12 });
	OaksLabDoor->SetMoveDirection(FTileVector::Up);
	OaksLabDoor->SetTargetBgm(RN::BgmOaksLab);
	OaksLabDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTPlayersHouseDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::PlayersHouseDoor);
	Setting.SetPoint({ 70, 141 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName(RN::RedDoor);
	Setting.SetHeight(1);

	AExteriorDoor* PlayersHouseDoor = SpawnEventTrigger<AExteriorDoor>(Setting);
	PlayersHouseDoor->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	PlayersHouseDoor->SetTargetPoint({ 3, 8 });
	PlayersHouseDoor->SetMoveDirection(FTileVector::Up);
	PlayersHouseDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTRivalsHouseDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::RivalshouseDoor);
	Setting.SetPoint({ 79, 141 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName("RedDoor");
	Setting.SetHeight(1);

	AExteriorDoor* RivalsHouseDoor = SpawnEventTrigger<AExteriorDoor>(Setting);
	RivalsHouseDoor->SetTargetMapName(Global::InteriorRivalsHouseLevel);
	RivalsHouseDoor->SetTargetPoint({ 4, 8 });
	RivalsHouseDoor->SetMoveDirection(FTileVector::Up);
	RivalsHouseDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTTechMan()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::TechMan);
	Setting.SetPoint({ 80, 151 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageNameAuto();

	ADialogueActor* TechMan = SpawnEventTrigger<ADialogueActor>(Setting);
	TechMan->SetDialogue({
		LR"(Technology is incredible!)",
		LR"(You can now store and recall items
and POKéMON as data via PC.)"
		});
	TechMan->SetTextColor(EFontColor::Blue);
	TechMan->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTGetStarterEventTriggers()
{
	std::function<bool()> Cond = []() {
		return false == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart);
	};

	UEventTargetSetting Setting0;
	Setting0.SetName(EN::GetStarterEventTrigger + "0");
	Setting0.SetPoint({ 76, 135 });

	std::vector<FTileVector> OakComePath0 = { Up, Up, Up, Up, Up, Right, Up };
	std::vector<FTileVector> OakGoToLabPath0 = { Down, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right, Right };
	std::vector<FTileVector> PlayerGoToLabPath0 = { Down, Down, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right };

	SpawnPTGetStarterEventTrigger(Setting0, Cond, OakComePath0, OakGoToLabPath0, PlayerGoToLabPath0);

	UEventTargetSetting Setting1;
	Setting1.SetName(EN::GetStarterEventTrigger + "1");
	Setting1.SetPoint({ 77, 135 });

	std::vector<FTileVector> OakComePath1 = { Up, Up, Up, Up, Up, Right, Right, Up };
	std::vector<FTileVector> OakGoToLabPath1 = { Down, Left, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right, Right };
	std::vector<FTileVector> PlayerGoToLabPath1 = { Down, Down, Left, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right };
	
	SpawnPTGetStarterEventTrigger(Setting1, Cond, OakComePath1, OakGoToLabPath1, PlayerGoToLabPath1);
}

void UExteriorPalletTownLevel::SpawnPTGetStarterEventTrigger(UEventTargetSetting _Setting, const std::function<bool()>& _Cond, const std::vector<FTileVector>& _OakComePath, const std::vector<FTileVector>& _OakGoToLabPath, const std::vector<FTileVector>& _PlayerGoToLabPath)
{
	AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(_Setting);
	Trigger->RegisterEvent(
		EEventTriggerAction::StepOn,
		_Cond,
		ES::Start(true)
		>> ES::FadeOutBgm(0.25f)
		>> ES::Wait(0.25f)
		>> ES::SetActive(GetName(), EN::Oak, true)
		>> ES::PlayBgm(RN::BgmOaksTheme)
		>> ES::FadeInBgm(0.5f)
		>> ES::Chat({ L"OAK: Hey! Wait!\nDon't go out!" }, EFontColor::Blue, 16)
		>> ES::ChangeDirection(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, FTileVector::Down)
		>> ES::PlaySE(RN::SESurprise)
		>> ES::Surprise(EN::PlayerCharacter)
		>> ES::Move(EN::Oak, _OakComePath, 3.6f, false)
		>> ES::Chat({
	   L"OAK: It's unsafe!\nWild POKéMON live in tall grass!",
	   L"You need your own POKéMON for\nyour protection.",
	   L"I know!\nHere, come with me!" }, EFontColor::Blue, 16)
	   >> ES::Move({ EN::Oak, EN::PlayerCharacter }, { _OakGoToLabPath, _PlayerGoToLabPath })
		>> ES::ChangeDirection(Global::ExteriorPalletTownLevel, EN::Oak, Up)
		>> ES::PlaySE(RN::SEDoorOpen)
		>> ES::PlayAnimation(EN::OaksLabDoor, "DoorOpen")
		>> ES::Move(EN::Oak, {Up})
		>> ES::SetActive(GetName(), EN::Oak, false)
		>> ES::Move(EN::PlayerCharacter, { Right, Up })
		>> ES::HideActor(Global::PlayerCharacter)
		>> ES::PlayAnimation(EN::OaksLabDoor, "DoorClose")
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::SetActive(Global::InteriorOaksLabLevel, EN::Oak, true)
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::Oak, { 6, 11 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::Oak, Up)
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::PlayerCharacter, { 6, 12 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::PlayerCharacter, Up)
		>> ES::ChangeLevel(Global::InteriorOaksLabLevel)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::Move(EN::Oak, { Up, Up, Up, Up, Up })
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::Oak, { 6, 3 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::Oak, Down)
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::RivalGreen, { 5, 4 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::RivalGreen, Up)
		>> ES::Move(EN::PlayerCharacter, { Up, Up, Up, Up, Up, Up, Up, Up })
		>> ES::FadeOutBgm(0.25f)
		>> ES::Wait(0.25f)
		>> ES::PlayBgm(RN::BgmOaksLab)
		>> ES::FadeInBgm(0.5f)
		>> ES::Chat({ L"GREEN: Gramps!\nI'm fed up with waiting!" }, EFontColor::Blue, 16)
		>> ES::Wait(0.5f)
		>> ES::Chat({
		L"OAK: GREEN?\nLet me think...",
		L"Oh, that's right, I told you to\ncome! Just wait!",
		L"Here, RED.",
		L"There are three POKéMON here.",
		L"The POKéMON are held inside\nthese POKé BALLS.",
		L"You can have one.\nGo on, choose!"
			}, EFontColor::Blue, 16)
		>> ES::Achieve(EAchievement::GetStarterEventStart)
		>> ES::End(true)
	);
}

void UExteriorPalletTownLevel::MakePTOak()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::Oak);
	Setting.SetPoint({ 75, 142 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageNameAuto();

	AEventTarget* Oak = SpawnEventTarget<AEventTarget>(Setting);
	Oak->SetActive(false);
}

void UExteriorPalletTownLevel::MakePTItemBalls()
{
	UEventTargetSetting PotionBallSetting;
	PotionBallSetting.SetName("PTPotionBall");
	PotionBallSetting.SetPoint({ 66, 121 });
	PotionBallSetting.SetDirection(FTileVector::Down);
	PotionBallSetting.SetCollidable(true);
	PotionBallSetting.SetImageName(RN::MapBall);
	PotionBallSetting.SetHeight(1);

	AItemBall* PotionBall = SpawnEventTrigger<AItemBall>(PotionBallSetting);
	PotionBall->SetItem(EItemId::Potion, 5);
	PotionBall->RegisterPredefinedEvent();

	UEventTargetSetting PokeBallSetting;
	PokeBallSetting.SetName("PTPokeBall");
	PokeBallSetting.SetPoint({ 67, 121 });
	PokeBallSetting.SetDirection(FTileVector::Down);
	PokeBallSetting.SetCollidable(true);
	PokeBallSetting.SetImageName(RN::MapBall);
	PokeBallSetting.SetHeight(1);

	AItemBall* PokeBall = SpawnEventTrigger<AItemBall>(PokeBallSetting);
	PokeBall->SetItem(EItemId::PokeBall, 10);
	PokeBall->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTAnimatedTiles()
{
	DrawFlowers({
	{69, 146}, {69, 147},
	{70, 146}, {70, 147},
	{71, 146}, {71, 147},
	{72, 146}, {72, 147},
		});

	DrawSeas({
		{71, 151}, {72, 151}, {73, 151}, {74, 151},
		{71, 152}, {72, 152}, {73, 152}, {74, 152},
		{71, 153}, {72, 153}, {73, 153}, {74, 153},
		{71, 154}, {72, 154}, {73, 154}, {74, 154},
		{71, 155}, {72, 155}, {73, 155}, {74, 155},
		{71, 156}, {72, 156}, {73, 156}, {74, 156},
		{71, 157}, {72, 157}, {73, 157}, {74, 157},
		{71, 158}, {72, 158}, {73, 158}, {74, 158},
		});
}

void UExteriorPalletTownLevel::MakeViridianCity()
{
	MakeVCPokemonCenterDoor();
	//MakeVCShopDoor();
	MakeVCShopClosedDoor();
	MakeVCTrainerSchoolClosedDoor();
	MakeVCPrivateHouseClosedDoor();
	MakeVCGymClosedDoor();
	MakeVCAnimatedTiles();
	MakeVCForestEntrances();
}

void UExteriorPalletTownLevel::MakeVCPokemonCenterDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::ViridianPokemonCenterDoor);
	Setting.SetPoint({ 78, 80 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName(RN::SlideDoor);
	Setting.SetHeight(1);

	AExteriorDoor* Door = SpawnEventTrigger<AExteriorDoor>(Setting);
	Door->SetTargetMapName(Global::InteriorPokemonCenterLevel);
	Door->SetTargetPoint({ 7, 8 });
	Door->SetTargetBgm(RN::BgmPokemonCenter);
	Door->SetMoveDirection(FTileVector::Up);
	Door->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakeVCShopDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::ViridianShopDoor);
	Setting.SetPoint({ 88, 73 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName(RN::SlideDoor);
	Setting.SetHeight(1);

	AExteriorDoor* Door = SpawnEventTrigger<AExteriorDoor>(Setting);
	Door->SetTargetMapName(Global::InteriorShopLevel);
	Door->SetTargetPoint({ 4, 7 });
	Door->SetMoveDirection(FTileVector::Up);
	Door->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakeVCShopClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("VCShopClosedDoor");
	Setting.SetPoint({ 88, 73 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakeVCTrainerSchoolClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("VCTrainerSchoolClosedDoor");
	Setting.SetPoint({ 77, 72 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakeVCPrivateHouseClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("VCPrivateHouseClosedDoor");
	Setting.SetPoint({ 77, 65 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakeVCGymClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("VCGymClosedDoor");
	Setting.SetPoint({ 88, 64 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakeVCAnimatedTiles()
{
	DrawFlowers({
		{80, 85}, {82, 85}, {84, 85}, {88, 85}, {90, 85}, {92, 85},
		{87, 80}, {89, 80}, {86, 79}, {88, 79}, {87, 78}, {89, 78}, {86, 77}, {88, 77}
	});

	DrawSeas({
		{63, 80}, {64, 80}, {65, 80}, {66, 80}, {67, 80}, {68, 80},
		{63, 81}, {64, 81}, {65, 81}, {66, 81}, {67, 81}, {68, 81},
		{63, 82}, {64, 82}, {65, 82}, {66, 82}, {67, 82}, {68, 82},
		{63, 83}, {64, 83}, {65, 83}, {66, 83}, {67, 83}, {68, 83},
		{63, 84}, {64, 84}, {65, 84}, {66, 84}, {67, 84}, {68, 84},
		});
}

void UExteriorPalletTownLevel::MakeVCForestEntrances()
{
	for (int i = 0; i < 2; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ForestEntrance" + std::to_string(i));
		Setting.SetPoint({69 + i, 25});
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		Trigger->RegisterEvent(
			EEventTriggerAction::StepOn,
			SKIP_CHECK,
			ES::Start(true)
			>> ES::FadeOutBgm(0.75f)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorViridianForestLevel)
			>> ES::ChangePoint(Global::ExteriorViridianForestLevel, EN::PlayerCharacter, {35, 62})
			>> ES::ChangeDirection(Global::ExteriorViridianForestLevel, Global::PlayerCharacter, FTileVector::Up)
			>> ES::CameraFocus(Global::PlayerCharacter)
			>> ES::ShowMapName(L"VIRIDIAN FOREST")
			>> ES::PlayBgm(RN::BgmViridianForest)
			>> ES::FadeInBgm(0.75f)
			>> ES::FadeIn(0.75f)
			>> ES::Wait(0.75f)
			>> ES::End(true)
		);
	}
}

void UExteriorPalletTownLevel::MakeRoute22()
{
	MakeR22AnimatedTiles();
	MakeR22ClosedDoor();
}

void UExteriorPalletTownLevel::MakeR22AnimatedTiles()
{
	DrawSeas({
		{26, 72}, {27, 72}, {28, 72}, {29, 72}, {30, 72}, {31, 72},
		{26, 73}, {27, 73}, {28, 73}, {29, 73}, {30, 73}, {31, 73},
		{26, 74}, {27, 74}, {28, 74}, {29, 74}, {30, 74}, {31, 74},
		{26, 75}, {27, 75}, {28, 75}, {29, 75}, {30, 75}, {31, 75},
		});
}

void UExteriorPalletTownLevel::MakeR22ClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("R22ClosedDoor");
	Setting.SetPoint({ 12, 69 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}


void UExteriorPalletTownLevel::MakePalletToRoute1AreaChanger()
{
	UEventTargetSetting Setting0;
	Setting0.SetName("PalletToRoute1AreaChanger0");
	Setting0.SetPoint({ 76, 133 });
	Setting0.SetHeight(1);

	UEventTargetSetting Setting1;
	Setting1.SetName("PalletToRoute1AreaChanger1");
	Setting1.SetPoint({ 77, 133 });
	Setting1.SetHeight(1);

	AEventTrigger* Changer0 = SpawnEventTrigger<AEventTrigger>(Setting0);
	AEventTrigger* Changer1 = SpawnEventTrigger<AEventTrigger>(Setting1);

	UEventStream& Stream = ES::Start(false)
		>> ES::ChangeArea("ROUTE 1", RN::BgmRoute1)
		>> ES::ShowMapName(L"ROUTE 1")
		>> ES::FadeOutBgm(0.25f)
		>> ES::Wait(0.25f)
		>> ES::PlayBgm(RN::BgmRoute1)
		>> ES::FadeInBgm(0.25f)
		>> ES::End(false);

	Changer0->RegisterEvent(EEventTriggerAction::StepOn, IsPlayerNotInRoute1, Stream);
	Changer1->RegisterEvent(EEventTriggerAction::StepOn, IsPlayerNotInRoute1, Stream);
}

void UExteriorPalletTownLevel::MakeRoute1ToPalletAreaChanger()
{
	UEventTargetSetting Setting0;
	Setting0.SetName("Route1ToPalletAreaChanger0");
	Setting0.SetPoint({ 76, 134 });
	Setting0.SetHeight(1);

	UEventTargetSetting Setting1;
	Setting1.SetName("Route1ToPalletAreaChanger1");
	Setting1.SetPoint({ 77, 134 });
	Setting1.SetHeight(1);

	AEventTrigger* Changer0 = SpawnEventTrigger<AEventTrigger>(Setting0);
	AEventTrigger* Changer1 = SpawnEventTrigger<AEventTrigger>(Setting1);

	UEventStream& Stream = ES::Start(false)
		>> ES::ChangeArea("PALLET TOWN", RN::BgmPalletTown)
		>> ES::ShowMapName(L"PALLET TOWN")
		>> ES::FadeOutBgm(0.25f)
		>> ES::Wait(0.25f)
		>> ES::PlayBgm(RN::BgmPalletTown)
		>> ES::FadeInBgm(0.25f)
		>> ES::End(false);

	Changer0->RegisterEvent(EEventTriggerAction::StepOn, IsPlayerNotInPalletTown, Stream);
	Changer1->RegisterEvent(EEventTriggerAction::StepOn, IsPlayerNotInPalletTown, Stream);
}

void UExteriorPalletTownLevel::MakeViridianToRoute1AreaChanger()
{
	for (int i = 0; i < 4; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ViridianCityToRoute1Changer" + std::to_string(i));
		Setting.SetPoint({ 74 + i, 94 });
		Setting.SetHeight(1);

		AEventTrigger* Changer = SpawnEventTrigger<AEventTrigger>(Setting);

		Changer->RegisterEvent(
			EEventTriggerAction::StepOn, 
			IsPlayerNotInRoute1,
			ES::Start(false)
			>> ES::ChangeArea("ROUTE 1", RN::BgmRoute1)
			>> ES::ShowMapName(L"ROUTE 1")
			>> ES::FadeOutBgm(0.25f)
			>> ES::Wait(0.25f)
			>> ES::PlayBgm(RN::BgmRoute1)
			>> ES::FadeInBgm(0.25f)
			>> ES::End(false));
	}
}

void UExteriorPalletTownLevel::MakeRoute1ToViridianAreaChanger()
{
	for (int i = 0; i < 4; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("Route1ToViridianCityChanger" + std::to_string(i));
		Setting.SetPoint({ 74 + i, 93 });
		Setting.SetHeight(1);

		AEventTrigger* Changer = SpawnEventTrigger<AEventTrigger>(Setting);

		Changer->RegisterEvent(
			EEventTriggerAction::StepOn,
			IsPlayerNotInViridianCity,
			ES::Start(false)
			>> ES::ChangeArea("VIRIDIAN CITY", RN::BgmViridianCity)
			>> ES::ShowMapName(L"VIRIDIAN CITY")
			>> ES::FadeOutBgm(0.25f)
			>> ES::Wait(0.25f)
			>> ES::PlayBgm(RN::BgmViridianCity)
			>> ES::FadeInBgm(0.25f)
			>> ES::End(false));
	}
}

void UExteriorPalletTownLevel::MakeViridianToRoute2AreaChanger()
{
	for (int i = 0; i < 5; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ViridianCityToRoute2Changer" + std::to_string(i));
		Setting.SetPoint({ 71 + i, 53 });
		Setting.SetHeight(1);

		AEventTrigger* Changer = SpawnEventTrigger<AEventTrigger>(Setting);

		Changer->RegisterEvent(
			EEventTriggerAction::StepOn,
			IsPlayerNotInRoute2,
			ES::Start(false)
			>> ES::ChangeArea("ROUTE 2", RN::BgmRoute2)
			>> ES::ShowMapName(L"ROUTE 2")
			>> ES::FadeOutBgm(0.25f)
			>> ES::Wait(0.25f)
			>> ES::PlayBgm(RN::BgmRoute2)
			>> ES::FadeInBgm(0.25f)
			>> ES::End(false));
	}
}

void UExteriorPalletTownLevel::MakeRoute2ToViridianAreaChanger()
{
	for (int i = 0; i < 5; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("Route2ToViridianCityChanger" + std::to_string(i));
		Setting.SetPoint({ 71 + i, 54 });
		Setting.SetHeight(1);

		AEventTrigger* Changer = SpawnEventTrigger<AEventTrigger>(Setting);

		Changer->RegisterEvent(
			EEventTriggerAction::StepOn,
			IsPlayerNotInViridianCity,
			ES::Start(false)
			>> ES::ChangeArea("VIRIDIAN CITY", RN::BgmViridianCity)
			>> ES::ShowMapName(L"VIRIDIAN CITY")
			>> ES::FadeOutBgm(0.25f)
			>> ES::Wait(0.25f)
			>> ES::PlayBgm(RN::BgmViridianCity)
			>> ES::FadeInBgm(0.25f)
			>> ES::End(false));
	}
}

std::string UExteriorPalletTownLevel::GetAreaNameStatic()
{
	APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(EN::PlayerCharacter);
	UExteriorPalletTownLevel* CurLevel = dynamic_cast<UExteriorPalletTownLevel*>(Player->GetWorld());
	if (nullptr == CurLevel)
	{
		MsgBoxAssert("UExteriorPalletTownLevel::IsPlayerInPalletTown 함수를 UExteriorPalletTownLevel 외부에서 호출했습니다.");
		return "";
	}

	return CurLevel->GetAreaName();
}

bool UExteriorPalletTownLevel::IsPlayerNotInPalletTown()
{
	return GetAreaNameStatic() != "PALLET TOWN";
}

bool UExteriorPalletTownLevel::IsPlayerNotInRoute1()
{
	return GetAreaNameStatic() != "ROUTE 1";
}

bool UExteriorPalletTownLevel::IsPlayerNotInRoute2()
{
	return GetAreaNameStatic() != "ROUTE 2";
}

bool UExteriorPalletTownLevel::IsPlayerNotInViridianCity()
{
	return GetAreaNameStatic() != "VIRIDIAN CITY";
}

