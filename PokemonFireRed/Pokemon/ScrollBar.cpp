#include "ScrollBar.h"
#include "PokemonUtil.h"

ABar::ABar() 
{
}

ABar::~ABar() 
{
}

void ABar::DrawHp(int _Value)
{
	float Prop = _Value / static_cast<float>(MaxValue);
	float TotalWidth = 48 * Global::FloatPixelSize;		// 빈 영역을 포함한 전체 영역의 너비
	float FillWidth = Prop * TotalWidth;				// 빨강, 노랑, 초록 바가 채워져 있는 영역의 너비

	std::string ImageName;
	if (Prop > 0.5f)
	{
		ImageName = RN::HpBarGreen;
	}
	else if (Prop > 0.2f)
	{ 
		ImageName = RN::HpBarYellow;
	}
	else
	{
		ImageName = RN::HpBarRed;
	}

	if (Renderers.size() > 0)
	{
		for (UImageRenderer* Renderer : Renderers)
		{
			Renderer->Destroy();
		}
		Renderers.clear();
	}

	{
		UImageRenderer* AreaRenderer = CreateImageRenderer(RenderingOrder);
		AreaRenderer->SetImage(RN::HpBarArea);

		FVector RenderScale = Global::HpBarAreaRenderScale;
		FVector Pos = RenderScale.Half2D();
		AreaRenderer->SetTransform({ Pos, RenderScale });
		Renderers.push_back(AreaRenderer);
	}

	{
		UImageRenderer* FillRenderer = CreateImageRenderer(RenderingOrder + 1);
		FillRenderer->SetImage(ImageName);

		FVector RenderScale = FVector(FillWidth, 3.0f * Global::FloatPixelSize);
		FVector Pos = RenderScale.Half2D();
		FillRenderer->SetTransform({ Pos, RenderScale });
		Renderers.push_back(FillRenderer);
	}
}

void ABar::DrawExp(int _Value)
{
	float Prop = _Value / static_cast<float>(MaxValue);
	int TotalPixelWidth = 64;
	int FillPixelWidth = UPokemonMath::Floor((Prop * TotalPixelWidth));

	std::string ImageName = RN::ExpBar;

	if (Renderers.size() > 0)
	{
		for (UImageRenderer* Renderer : Renderers)
		{
			Renderer->Destroy();
		}
		Renderers.clear();
	}

	{
		UImageRenderer* AreaRenderer = CreateImageRenderer(RenderingOrder);
		AreaRenderer->SetImage(RN::ExpBarArea);

		FVector RenderScale = Global::ExpBarAreaRenderScale;
		FVector Pos = RenderScale.Half2D();
		AreaRenderer->SetTransform({ Pos, RenderScale });
		Renderers.push_back(AreaRenderer);
	}

	{
		UImageRenderer* FillRenderer = CreateImageRenderer(RenderingOrder + 1);
		FillRenderer->SetImage(ImageName);

		UWindowImage* FillImage = FillRenderer->GetImage();
		FillImage->SetCuttingTransform({ {0, 0}, {FillPixelWidth, 3} });

		FVector RenderScale = FVector(FillPixelWidth, 3) * Global::FloatPixelSize;
		FVector Pos = RenderScale.Half2D();
		FillRenderer->SetTransform({ Pos, RenderScale });
		Renderers.push_back(FillRenderer);
	}
}

