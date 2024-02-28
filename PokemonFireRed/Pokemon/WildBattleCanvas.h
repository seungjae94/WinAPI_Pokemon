#pragma once
#include "Canvas.h"
#include "Pokemon.h"

class AWildBattleCanvas : public ACanvas
{
public:
	// constructor destructor
	AWildBattleCanvas();
	~AWildBattleCanvas();

	// delete Function
	AWildBattleCanvas(const AWildBattleCanvas& _Other) = delete;
	AWildBattleCanvas(AWildBattleCanvas&& _Other) noexcept = delete;
	AWildBattleCanvas& operator=(const AWildBattleCanvas& _Other) = delete;
	AWildBattleCanvas& operator=(AWildBattleCanvas&& _Other) noexcept = delete;

	void BattleStart(const UPokemon& _PlayerPokemon, const UPokemon& _EnemyPokemon);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Background = nullptr;

	// 최상위 요소
	UImageRenderer* MsgBox = nullptr;
	UImageRenderer* ActionBox = nullptr;
	UImageRenderer* EnemyPokemonBox = nullptr;
	UImageRenderer* PlayerPokemonBox = nullptr;
	UImageRenderer* EnemyGround = nullptr;
	UImageRenderer* PlayerGround = nullptr;

	// EnemyPokemonBox 요소
	AText* EnemyPokemonNameText = nullptr;
	AText* EnemyPokemonLevelText = nullptr;
	AScrollBar* EnemyPokemonHpBar = nullptr;
	AImageElement* EnemyPokemonGenderMark = nullptr;

	// PlayerPokemonBox 요소
	AText* PlayerPokemonNameText = nullptr;
	AText* PlayerPokemonLevelText = nullptr;
	AText* PlayerPokemonCurHpText = nullptr;
	AText* PlayerPokemonHpText = nullptr;
	AScrollBar* PlayerPokemonHpBar = nullptr;
	AScrollBar* PlayerPokemonExpBar = nullptr;
	AImageElement* PlayerPokemonGenderMark = nullptr;

	// EnemyPokemonGround 요소
	APokemonElement* EnemyPokemonImage = nullptr;

	// PlayerPokemonGround 요소
	AAnimationElement* PlayerBattler = nullptr;
	APokemonElement* PlayerPokemonImage = nullptr;
};

