#pragma once
#include "MapLevel.h"

class UEventTargetSetting;

class UExteriorPalletTownLevel : public UMapLevel
{
public:
	// constructor destructor
	UExteriorPalletTownLevel();
	~UExteriorPalletTownLevel();

	// delete Function
	UExteriorPalletTownLevel(const UExteriorPalletTownLevel& _Other) = delete;
	UExteriorPalletTownLevel(UExteriorPalletTownLevel&& _Other) noexcept = delete;
	UExteriorPalletTownLevel& operator=(const UExteriorPalletTownLevel& _Other) = delete;
	UExteriorPalletTownLevel& operator=(UExteriorPalletTownLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void LevelStart(ULevel* _PrevLevel) override;
private:
	const FTileVector Up = FTileVector::Up;
	const FTileVector Down = FTileVector::Down;
	const FTileVector Left = FTileVector::Left;
	const FTileVector Right = FTileVector::Right;

	// 태초 마을
	void MakePalletTown();
	void MakePTOaksLabDoor();
	void MakePTPlayersHouseDoor();
	void MakePTRivalsHouseDoor();
	void MakePTTechMan();
	void MakePTGetStarterEventTriggers();
	void SpawnPTGetStarterEventTrigger(
		UEventTargetSetting _Setting,
		const std::function<bool()>& _Cond,
		const std::vector<FTileVector>& _OakComePath, 
		const std::vector<FTileVector>& _OakGoToLabPath, 
		const std::vector<FTileVector>& _PlayerGoToLabPath);
	void MakePTOak();
	void MakePTItemBalls();
	void MakePTAnimatedTiles();

	// 상록 시티
	void MakeViridianCity();
	void MakeVCPokemonCenterDoor();
	void MakeVCShopDoor();
	void MakeVCShopClosedDoor();
	void MakeVCTrainerSchoolClosedDoor();
	void MakeVCPrivateHouseClosedDoor();
	void MakeVCGymClosedDoor();
	void MakeVCAnimatedTiles();
	void MakeVCForestEntrances();

	// 22번 도로
	void MakeRoute22();
	void MakeR22AnimatedTiles();
	void MakeR22ClosedDoor();

	// 지역 변경
	void MakePalletToRoute1AreaChanger();
	void MakeRoute1ToPalletAreaChanger();
	void MakeViridianToRoute1AreaChanger();
	void MakeRoute1ToViridianAreaChanger();
	void MakeViridianToRoute2AreaChanger();
	void MakeRoute2ToViridianAreaChanger();

	// 체크 함수
	static std::string GetAreaNameStatic();
	static bool IsPlayerNotInPalletTown();
	static bool IsPlayerNotInRoute1();
	static bool IsPlayerNotInRoute2();
	static bool IsPlayerNotInViridianCity();
};

