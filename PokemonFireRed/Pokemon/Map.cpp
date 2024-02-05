#include "Map.h"

#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "Player.h"

AMap::AMap()
{
}

AMap::~AMap()
{
}

void AMap::SetBackgroundImage(std::string_view _Name)
{
	BackgroundRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	RenderScale = Scale * Global::F_PIXEL_SIZE;

	TileCount.X = RenderScale.X / Global::TILE_SIZE;
	TileCount.Y = RenderScale.Y / Global::TILE_SIZE;

	BackgroundRenderer->SetTransform({ {0, 0}, RenderScale });
	BackgroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::BeginPlay()
{
	AActor::BeginPlay();

	BackgroundRenderer = CreateImageRenderer(-1000);
}

void AMap::Tick(float _DeltaTime)
{
	static bool FirstUpdate = false;

	AActor::Tick(_DeltaTime);

	if (false == FirstUpdate)
	{
		FirstUpdate = true;
		SyncGroundScreenPosToPlayerWorldPos();
	}

}

void AMap::SyncGroundScreenPosToPlayerWorldPos()
{
	FVector ScreenPos = { Global::HALF_SCREEN_X, Global::HALF_SCREEN_Y };
	ScreenPos += RenderScale * 0.5f;										// ��� �»�ܰ� ȭ�� �߾��� ��ġ��Ų��.
	ScreenPos += LTWorldPos * Global::F_TILE_SIZE;							// ��� �»���� ���� ��ǥ�� ���Ѵ�.
	ScreenPos -= Player->GetWorldPos() * Global::F_TILE_SIZE;				// �÷��̾��� ���� ��ǥ�� ����.
	SetActorLocation(ScreenPos);
}