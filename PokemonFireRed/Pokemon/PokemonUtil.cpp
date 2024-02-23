#include "PokemonUtil.h"
#include <sstream>
#include "Global.h"
#include "Pokemon.h"

UPokemonUtil::UPokemonUtil()
{
}

UPokemonUtil::~UPokemonUtil()
{
}

std::vector<int> UPokemonUtil::FrameCountVectorToIndexVector(std::vector<int> _FrameCounts)
{
	std::vector<int> Indexes;

	int Index = 0;
	for (int FrameCount : _FrameCounts)
	{
		for (int i = 0; i < FrameCount; i++)
		{
			Indexes.push_back(Index);
		}
		Index++;
	}

	return Indexes;
}

FVector UPokemonUtil::GetImageScale(UImageRenderer* _Renderer)
{
	UWindowImage* Image = _Renderer->GetImage();
	return Image->GetScale();
}

FVector UPokemonUtil::GetRenderScale(UImageRenderer* _Renderer)
{
	UWindowImage* Image = _Renderer->GetImage();
	return Image->GetScale() * Global::FloatPixelSize;
}

void UPokemonUtil::CreateMiniPokemonAnimations(UImageRenderer* _Renderer)
{
	if (nullptr == _Renderer->GetImage())
	{
		_Renderer->SetImage(Global::PokemonMiniImageName);
	}

	for (EPokedexNo No : UPokemon::GetImplementedSpeciesNo())
	{
		int Idx = static_cast<int>(No);
		_Renderer->CreateAnimation(Global::PokemonMiniPrefix + UPokemon::GetSpeciesName(No), Global::PokemonMiniImageName, Idx * 2 - 2, Idx * 2 - 1, 1.0f / 6, true);
	}
}

std::vector<std::string> UPokemonUtil::StringSplit(const std::string& _Str, char _Delimeter)
{
	std::vector<std::string> Result;
	std::stringstream Stream(_Str);
	std::string Buffer;

	while (std::getline(Stream, Buffer, _Delimeter))
	{
		Result.push_back(Buffer);
	}

	return Result;
}

std::vector<std::wstring> UPokemonUtil::StringSplit(std::wstring_view _Str, wchar_t _Delimeter)
{
	std::vector<std::wstring> Result;
	std::wstringstream Stream(_Str.data());
	std::wstring Buffer;

	while (std::getline(Stream, Buffer, _Delimeter))
	{
		Result.push_back(Buffer);
	}

	return Result;
}
