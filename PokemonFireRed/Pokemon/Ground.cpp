#include "Ground.h"

AGround::AGround()
{
}

AGround::~AGround()
{
}

void AGround::BeginPlay()
{
	AActor::BeginPlay();

	SetActorLocation({ 16*(10), 16*(10) });

	Renderer = CreateImageRenderer(0);
	Renderer->SetImage("PalletTown.png");
	Renderer->SetTransform({ {0, 0}, {384*3, 365*3} });
	Renderer->SetImageCuttingTransform({ {0, 0}, {384, 365} });
}

void AGround::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}