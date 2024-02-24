#include "ImageElement.h"
#include "PokemonUtil.h"

AImageElement::AImageElement()
{
}

AImageElement::~AImageElement()
{
}

void AImageElement::SetImage(std::string_view _ImageName)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		Renderer->CameraEffectOff();
	}

	Renderer->SetImage(_ImageName);
	FVector RenderScale = UPokemonUtil::GetRenderScale(Renderer);
	FVector Pos = FVector::Zero;
	Renderer->SetTransform({ Pos, RenderScale });
}
