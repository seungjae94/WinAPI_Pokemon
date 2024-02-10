#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class UEventManager;

enum class ETargetMoveState
{
	Idle,
	Walk
};

// 이벤트에 의해 강제로 행동할 수 있는 액터
// - ex) 플레이어, NPC, 표지판, 바위 등
class AEventTarget : public AActor
{
	friend UEventManager;
public:
	// constructor destructor
	AEventTarget();
	~AEventTarget();

	// delete Function
	AEventTarget(const AEventTarget& _Other) = delete;
	AEventTarget(AEventTarget&& _Other) noexcept = delete;
	AEventTarget& operator=(const AEventTarget& _Other) = delete;
	AEventTarget& operator=(AEventTarget&& _Other) noexcept = delete;

	void ChangeAnimation(ETargetMoveState _State, const FTileVector& _Direction);

	FTileVector GetDirection() const
	{
		return Direction;
	}

	void SetDirection(const FTileVector& _Direction)
	{
		Direction = _Direction;
	}

	void SetMoveState(ETargetMoveState _MoveState)
	{
		MoveState = _MoveState;
	}

	ETargetMoveState GetMoveState() const
	{
		return MoveState;
	}

protected:
	// 이동 관련 변수
	FTileVector Direction = FTileVector::Down;
	FVector PrevPos;
	FVector NextPos;
	ETargetMoveState MoveState = ETargetMoveState::Idle;
	float MoveTime = 0.0f;
	float Timer = 0.0f;
	int MoveIndex = -1;
	bool Rotatable = false;
	bool Walkable = false;

	// 렌더러
	UImageRenderer* Renderer = nullptr;

	// 초기화 함수
	// - BeginPlay는 SpawnActor를 실행할 때 자동으로 호출되기 때문에 이름, 위치 등 내가 원하는 값을 세팅하기 전에 호출된다.
	// - InitEventTarget으로 명시적으로 
	virtual void InitEventTarget(FTileVector _Direction, bool _Rotatable, bool _Walkable) {};
private:
};

