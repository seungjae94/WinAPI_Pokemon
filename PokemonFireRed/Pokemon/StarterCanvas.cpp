#include "StarterCanvas.h"

AStarterCanvas::AStarterCanvas()
{
}

AStarterCanvas::~AStarterCanvas()
{
}

void AStarterCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	PokemonBackground = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 81, 25);
	PokemonBackground->SetImage(RN::StarterBackground);

	PokemonImage = CreateImageElement(PokemonBackground, ERenderingOrder::UI1, EPivotType::LeftTop, 7, 7, EImageElementType::PokemonFront);
	PokemonImage->SetPokemon(EPokemonId::Pidgey);

	OptionBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::RightBot, -17, -49);
	OptionBox->SetImage(RN::StarterOptionBox);
}
