#pragma once
#include <string>
#include <EngineBase/EngineMath.h>
#include "Global.h"

enum class EDirection
{
	Down,
	Up,
	Left,
	Right,
	None
};

struct FTileVector
{
public:
	const static FTileVector Zero;
	const static FTileVector Up;
	const static FTileVector Down;
	const static FTileVector Left;
	const static FTileVector Right;

	int X = 0;
	int Y = 0;

	FTileVector()
	{
	}

	FTileVector(int _X, int _Y)
		: X(_X), Y(_Y)
	{
	}

	bool operator ==(const FTileVector& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	bool operator !=(const FTileVector& _Other) const
	{
		return X != _Other.X || Y != _Other.Y;
	}

	FTileVector operator +(const FTileVector& _Other) const
	{
		FTileVector NewPoint = *this;
		NewPoint.X += _Other.X;
		NewPoint.Y += _Other.Y;
		return NewPoint;
	}

	FTileVector& operator +=(const FTileVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}


	FVector ToFVector() const
	{
		return FVector(Global::TILE_SIZE * X, Global::TILE_SIZE * Y);
	}

	std::string ToString() const
	{
		if (*this == Up)
		{
			return "Up";
		}
		else if (*this == Down)
		{
			return "Down";
		}
		else if (*this == Left)
		{
			return "Left";
		}
		else if (*this == Right)
		{
			return "Right";
		}

		return "None";
	}
};

class PokemonMath
{
public:
	// constructor destructor
	PokemonMath();
	~PokemonMath();

	// delete Function
	PokemonMath(const PokemonMath& _Other) = delete;
	PokemonMath(PokemonMath&& _Other) noexcept = delete;
	PokemonMath& operator=(const PokemonMath& _Other) = delete;
	PokemonMath& operator=(PokemonMath&& _Other) noexcept = delete;

	static FVector Lerp(const FVector& _Start, const FVector& _End, float _t);

protected:

private:

};

