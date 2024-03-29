#pragma once
#include "BattleStateMachine.h"
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"
#include "Battler.h"
#include "BattleActionStateMachine.h"
#include "BattleEOTStateMachine.h"
#include "BattleFaintStateMachine.h"

class UBattleLevel;
class ABattleCanvas;
class APokemonMsgBox;
struct FMoveEffectTestResult;

enum class EBattleEndReason
{
	None,
	WinToWild,
	WinToTrainer,
	LoseToWild,
	LoseToTrainer,
	CatchSuccess
};

// 턴 순서를 관리하는 역할
class ABattleTurnStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		Action1,
		Action1Faint,
		TestAction2,
		Action2,
		Action2Faint,
		StartEndOfTurn,
		TestEndOfTurn1,
		EndOfTurn1,
		EndOfTurn1Faint,
		TestEndOfTurn2,
		EndOfTurn2,
		EndOfTurn2Faint,
		End
	};

	enum class EFaintState
	{
		None,
		HidePokemon,
		ShowFaintMessage,
	};
public:
	// constructor destructor
	ABattleTurnStateMachine();
	~ABattleTurnStateMachine();

	// delete Function
	ABattleTurnStateMachine(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine(ABattleTurnStateMachine&& _Other) noexcept = delete;
	ABattleTurnStateMachine& operator=(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine& operator=(ABattleTurnStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	EBattleEndReason WhyEnd() const
	{
		return Reason;
	}

	void SetBASM(ABattleActionStateMachine* _BASM)
	{
		BattleActionSM = _BASM;
	}

	void SetEOTSM(ABattleEOTStateMachine* _EOTSM)
	{
		BattleEOTSM = _EOTSM;
	}

	void SetFSM(ABattleFaintStateMachine* _FSM)
	{
		BattleFaintSM = _FSM;
	}

	void Start() override;

protected:

private:
	// 상태 틱 함수
	void ProcessAction1();
	void ProcessAction1Faint();  // Faint 처리를 다 하고 나서 배틀 종료, 페이즈 스킵 등의 처리
	void ProcessTestAction2();
	void ProcessAction2();
	void ProcessAction2Faint();
	void ProcessTestEndOfTurn1();
	void ProcessStartEndOfTurn();
	void ProcessEndOfTurn1();
	void ProcessEndOfTurn1Faint();
	void ProcessTestEndOfTurn2();
	void ProcessEndOfTurn2();
	void ProcessEndOfTurn2Faint();

	// 고유 데이터
	ESubstate State = ESubstate::None;
	EBattleEndReason Reason = EBattleEndReason::None;

	float Timer = 0.0f;
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;
	UBattler* EOTTarget = nullptr;
	UBattler* EOTNextTarget = nullptr;
	bool PlayerFaintChecked = false;
	bool EnemyFaintChecked = false;

	// 로직
	void Tick(float _DeltaTime) override;

	// 유틸 함수
	void SetPlayerAsAttacker();
	void SetEnemyAsAttacker();
	void SetPlayerAsEOTTarget();
	void SetEnemyAsEOTTarget();
	void SwapAttackerAndDefender();
	void EndTurnWithReason();
	bool& GetAttackerFaintChecked();
	bool& GetDefenderFaintChecked();
	bool& GetEOTTargetFaintChecked();
	bool& GetEOTNextTargetFaintChecked();

	// SM
	ABattleActionStateMachine* BattleActionSM = nullptr;
	ABattleEOTStateMachine* BattleEOTSM = nullptr;
	ABattleFaintStateMachine* BattleFaintSM = nullptr;
};

