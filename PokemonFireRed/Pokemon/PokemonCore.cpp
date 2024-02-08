#include "PokemonCore.h"
#include "TitleLevel.h"
#include "ExteriorPalletTownLevel.h"
#include "InteriorOaksLabLevel.h"
#include "Global.h"

PokemonCore::PokemonCore()
{
}

PokemonCore::~PokemonCore()
{
}

void PokemonCore::BeginPlay()
{
	// 게임 기본 설정
	MainWindow.SetWindowScale({ Global::SCREEN_X, Global::SCREEN_Y });
	MainWindow.SetClearColor(Color8Bit::BlackA);

	// 레벨 생성
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UExteriorPalletTownLevel>("ExteriorPalletTownLevel");
	CreateLevel<UInteriorOaksLabLevel>("InteriorOaksLabLevel");

	// 시작 레벨 설정
	//ChangeLevel("TitleLevel");
	ChangeLevel("ExteriorPalletTownLevel");
}
