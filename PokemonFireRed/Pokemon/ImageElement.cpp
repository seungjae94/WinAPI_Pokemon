#include "ImageElement.h"
#include "PokemonUtil.h"

AImageElement::AImageElement()
{
}

AImageElement::~AImageElement()
{
}

void AImageElement::SetRenderingOrder(ERenderingOrder _Order)
{
	AUIElement::SetRenderingOrder(_Order);

	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
	}

	Renderer->SetOrder(_Order);
}

void AImageElement::SetImage(std::string_view _ImageName)
{
	ImageName = _ImageName;

	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
	}

	Renderer->SetImage(_ImageName);
	UPokemonUtil::AlignImage(Renderer, PivotType);
	OriginalRenderScale = UPokemonUtil::GetRenderScale(Renderer);
}

void AImageElement::SetPokemon(EPokemonId _Id)
{
	if (nullptr == Renderer || false == MadePokemonAnimation)
	{
		switch (Type)
		{
		case EImageElementType::PokemonMiniMove:
			SetImage(RN::PokemonMiniPlaceHolder);
			UPokemonUtil::CreatePokemonMiniMoveAnimations(Renderer);
			break;
		case EImageElementType::PokemonMiniStop:
			SetImage(RN::PokemonMiniPlaceHolder);
			UPokemonUtil::CreatePokemonMiniStopAnimations(Renderer);
			break;
		case EImageElementType::PokemonFront:
			SetImage(RN::PokemonFrontPlaceHolder);
			UPokemonUtil::CreatePokemonFrontAnimations(Renderer);
			break;
		case EImageElementType::PokemonBack:
			SetImage(RN::PokemonBackPlaceHolder);
			UPokemonUtil::CreatePokemonBackAnimations(Renderer);
			break;
		default:
			break;
		}

		MadePokemonAnimation = true;
	}

	std::string AnimPrefix;
	switch (Type)
	{
	case EImageElementType::PokemonMiniMove:
		AnimPrefix = Global::PokemonMiniMovePrefix;
		break;
	case  EImageElementType::PokemonMiniStop:
		AnimPrefix = Global::PokemonMiniStopPrefix;
		break;
	case EImageElementType::PokemonFront:
		AnimPrefix = Global::PokemonFrontPrefix;
		break;
	case EImageElementType::PokemonBack:
		AnimPrefix = Global::PokemonBackPrefix;
		break;
	default:
		MsgBoxAssert("포켓몬 이미지 타입을 설정하지 않았습니다.");
		break;
	}

	const FPokemonSpecies* Species = UGameData::FindSpecies(_Id);
	Renderer->ChangeAnimation(AnimPrefix + Species->Name);
}

void AImageElement::SetPokemon(const UPokemon* _Pokemon)
{
	SetPokemon(_Pokemon->GetPokedexNo());
}

void AImageElement::SetPokemon(const UPokemon& _Pokemon)
{
	SetPokemon(&_Pokemon);
}


FTransform AImageElement::GetUITransform()
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
	}

	return Renderer->GetActorBaseTransform();
}

void AImageElement::CreateAnimation(std::string_view _AnimName, int _Start, int _End, float _Interval, bool _Loop)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 렌더러가 존재하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 이미지를 지정하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Start, _End, _Interval, _Loop);
}

void AImageElement::CreateAnimation(std::string_view _AnimName, const std::vector<int>& _Indexs, std::vector<float> _Times, bool _Loop)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 렌더러가 존재하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 이미지를 지정하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Indexs, _Times, _Loop);
}

void AImageElement::ChangeAnimation(std::string_view _AnimName, bool _IsForce)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::ChangeAnimation 실패. 렌더러가 존재하지 않아 애니메이션 " + _AnimName.data() + "을 재생할 수 없습니다.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::ChangeAnimation 실패. 이미지를 지정하지 않아 애니메이션 " + _AnimName.data() + "을 재생할 수 없습니다.");
		return;
	}

	Renderer->ChangeAnimation(_AnimName, _IsForce);
}

bool AImageElement::IsCurAnimationEnd()
{
	return Renderer->IsCurAnimationEnd();
}
