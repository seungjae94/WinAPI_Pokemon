#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "MoveEffectTester.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"
#include "Battler.h"
#include "BattleUtil.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start()
{
	ABattleStateMachine::Start();

	// 적 액션 생성 및 데이터 초기화
	UBattleEnemyActionGenerator::Generate(Enemy);
	Reason = EBattleEndReason::None;
	PlayerFaintChecked = false;
	EnemyFaintChecked = false;

	// 턴 순서 결정
	bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(Player, Enemy);
	if (true == IsPlayerFirst)
	{
		SetPlayerAsAttacker();
	}
	else
	{
		SetEnemyAsAttacker();
	}

	State = ESubstate::Action1;
	BattleActionSM->Start(Attacker, Defender);
}

void ABattleTurnStateMachine::ProcessAction1()
{
	if (true == BattleActionSM->IsEnd())
	{
		// 포획에 성공한 경우 즉시 배틀을 종료해야 한다.
		if (true == Player->GetCatchResult())
		{
			EndTurnWithReason();
			return;
		}

		State = ESubstate::Action1Faint;

		BattleFaintSM->Start(Attacker, Defender, GetAttackerFaintChecked(), GetDefenderFaintChecked());
	}
}

void ABattleTurnStateMachine::ProcessAction1Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 즉시 배틀을 종료해야 한다.
		// 즉시 배틀을 종료해야 하기 때문에 턴도 즉시 종료한다.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// 턴은 계속 진행하지만 Action2를 수행할 수 있는지는 확인을 해줘야 한다.
		State = ESubstate::TestAction2;
		SwapAttackerAndDefender();
	}
}

void ABattleTurnStateMachine::ProcessTestAction2()
{
	// Attacker나 Defender가 Faint 상태라면 Action2를 수행할 수 없다.
	if (true == Attacker->CurPokemon()->IsFaint() || true == Defender->CurPokemon()->IsFaint())
	{
		State = ESubstate::StartEndOfTurn;
		return;
	}

	// 그 외의 경우 Action2를 수행한다.
	State = ESubstate::Action2;
	BattleActionSM->Start(Attacker, Defender);
}

void ABattleTurnStateMachine::ProcessAction2()
{
	if (true == BattleActionSM->IsEnd())
	{
		State = ESubstate::Action2Faint;

		bool AttackerFaintChecked = GetAttackerFaintChecked();
		bool DefenderFaintChecked = GetDefenderFaintChecked();
		BattleFaintSM->Start(Attacker, Defender, GetAttackerFaintChecked(), GetDefenderFaintChecked());
	}
}

void ABattleTurnStateMachine::ProcessAction2Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 즉시 종료해야 한다.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// EOT 단계로 넘어간다.
		State = ESubstate::StartEndOfTurn;
	}
}

void ABattleTurnStateMachine::ProcessStartEndOfTurn()
{
	// EOT 순서를 결정한다.
	bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirstEOT(Player, Enemy);

	if (true == IsPlayerFirst)
	{
		SetPlayerAsEOTTarget();
	}
	else
	{
		SetEnemyAsEOTTarget();
	}

	// EOT1를 수행할 수 있는지 체크한다.
	State = ESubstate::TestEndOfTurn1;
}

void ABattleTurnStateMachine::ProcessTestEndOfTurn1()
{
	// EOTTarget이 Faint 상태라면 EOT1을 수행할 수 없다.
	if (true == EOTTarget->CurPokemon()->IsFaint())
	{
		State = ESubstate::TestEndOfTurn2;
		return;
	}

	// 그 외의 경우 EOT1을 수행한다.
	State = ESubstate::EndOfTurn1;

	BattleEOTSM->Start(EOTTarget, EOTNextTarget);
}

void ABattleTurnStateMachine::ProcessEndOfTurn1()
{
	if (true == BattleEOTSM->IsEnd())
	{
		State = ESubstate::EndOfTurn1Faint;
		BattleFaintSM->Start(EOTNextTarget, EOTTarget, GetEOTNextTargetFaintChecked(), GetEOTTargetFaintChecked());
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn1Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 즉시 종료해야 한다.
		if (true == EOTTarget->AllFaint() || true == EOTNextTarget->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// EOT2를 수행할 수 있는지 체크한다.
		State = ESubstate::TestEndOfTurn2;
	}
}

void ABattleTurnStateMachine::ProcessTestEndOfTurn2()
{
	// EOTNextTarget이 Faint 상태라면 EOT2를 수행할 수 없다.
	if (true == EOTNextTarget->CurPokemon()->IsFaint())
	{
		State = ESubstate::End;
		return;
	}

	// 그 외의 경우 EOT2를 수행한다.
	State = ESubstate::EndOfTurn2;
	BattleEOTSM->Start(EOTNextTarget, EOTTarget);
}

void ABattleTurnStateMachine::ProcessEndOfTurn2()
{
	if (true == BattleEOTSM->IsEnd())
	{
		State = ESubstate::EndOfTurn2Faint;
		BattleFaintSM->Start(EOTTarget, EOTNextTarget, GetEOTTargetFaintChecked(), GetEOTNextTargetFaintChecked());
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn2Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// 어느 한 쪽이 전부 죽을 경우 턴 종료 이유를 기록하고 종료해야 한다.
		if (true == EOTTarget->AllFaint() || true == EOTNextTarget->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		State = ESubstate::End;
	}
}

void ABattleTurnStateMachine::SwapAttackerAndDefender()
{
	UBattler* Tmp = Attacker;
	Attacker = Defender;
	Defender = Tmp;
}

void ABattleTurnStateMachine::EndTurnWithReason()
{
	if (true == Player->GetCatchResult())
	{
		Reason = EBattleEndReason::CatchSuccess;
		Player->SetCatchResult(false);
	}
	else if (true == Enemy->AllFaint())
	{
		if (true == Enemy->IsWildPokemon())
		{
			Reason = EBattleEndReason::WinToWild;
		}
		else
		{
			Reason = EBattleEndReason::WinToTrainer;
		}
	}
	else if (true == Player->AllFaint())
	{
		if (true == Enemy->IsWildPokemon())
		{
			Reason = EBattleEndReason::LoseToWild;
		}
		else
		{
			Reason = EBattleEndReason::LoseToTrainer;
		}
	}

	State = ESubstate::End;
}

bool& ABattleTurnStateMachine::GetAttackerFaintChecked()
{
	if (Attacker == Player)
	{
		return PlayerFaintChecked;
	}

	return EnemyFaintChecked;
}

bool& ABattleTurnStateMachine::GetDefenderFaintChecked()
{
	if (Defender == Player)
	{
		return PlayerFaintChecked;
	}

	return EnemyFaintChecked;
}

bool& ABattleTurnStateMachine::GetEOTTargetFaintChecked()
{
	if (EOTTarget == Player)
	{
		return PlayerFaintChecked;
	}

	return EnemyFaintChecked;
}

bool& ABattleTurnStateMachine::GetEOTNextTargetFaintChecked()
{
	if (EOTNextTarget == Player)
	{
		return PlayerFaintChecked;
	}

	return EnemyFaintChecked;
}

void ABattleTurnStateMachine::SetPlayerAsAttacker()
{
	Attacker = Player;
	Defender = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsAttacker()
{
	Attacker = Enemy;
	Defender = Player;
}

void ABattleTurnStateMachine::SetPlayerAsEOTTarget()
{
	EOTTarget = Player;
	EOTNextTarget = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsEOTTarget()
{
	EOTTarget = Enemy;
	EOTNextTarget = Player;
}


void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleTurnStateMachine::ESubstate::None:
		break;
	case ABattleTurnStateMachine::ESubstate::Action1:
		ProcessAction1();
		break;
	case ABattleTurnStateMachine::ESubstate::Action1Faint:
		ProcessAction1Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::TestAction2:
		ProcessTestAction2();
		break;
	case ABattleTurnStateMachine::ESubstate::Action2:
		ProcessAction2();
		break;
	case ABattleTurnStateMachine::ESubstate::Action2Faint:
		ProcessAction2Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::StartEndOfTurn:
		ProcessStartEndOfTurn();
		break;
	case ABattleTurnStateMachine::ESubstate::TestEndOfTurn1:
		ProcessTestEndOfTurn1();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn1:
		ProcessEndOfTurn1();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn1Faint:
		ProcessEndOfTurn1Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::TestEndOfTurn2:
		ProcessTestEndOfTurn2();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn2:
		ProcessEndOfTurn2();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn2Faint:
		ProcessEndOfTurn2Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}