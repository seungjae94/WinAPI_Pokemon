﻿#include "BattlePokeBallStateMachine.h"
#include "Battler.h"
#include "PokemonMsgBox.h"
#include "BattleCanvas.h"

ABattlePokeBallStateMachine::ABattlePokeBallStateMachine() 
{
}

ABattlePokeBallStateMachine::~ABattlePokeBallStateMachine() 
{
}

void ABattlePokeBallStateMachine::Start()
{
	ABattleStateMachine::Start();

	State = ESubstate::BallUseMessage;
	MsgBox->SetMessage(L"RED used\nPOKé BALL!");
	MsgBox->Write();
}

bool ABattlePokeBallStateMachine::IsEnd() const
{
	return State == ESubstate::End;
}

void ABattlePokeBallStateMachine::Tick(float _DeltaTime)
{
	ABattleStateMachine::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::BallUseMessage:
		ProcessBallUseMessage();
		break;
	case ESubstate::PokeBallThrow:
		ProcessPokeBallThrow(_DeltaTime);
		break;
	case ESubstate::PokeBallBlocked:
		ProcessPokeBallBlocked();
		break;
	case ESubstate::DontBeAThiefMessage:
		ProcessDontBeAThiefMessage();
		break;
	case ESubstate::PokeBallPullInPokemon:
		ProcessPokeBallPullInPokemon();
		break;
	case ESubstate::PokeBallClosing:
		ProcessPokeBallClosing();
		break;
	case ESubstate::PokeBallVerticalMove:
		ProcessPokeBallVerticalMove();
		break;
	case ESubstate::TestCatch:
		ProcessTestCatch();
		break;
	case ESubstate::CatchResultAnim:
		ProcessCatchResultAnim();
		break;
	case ESubstate::CatchFailMessage:
		ProcessCatchFailMessage();
		break;
	case ESubstate::CatchSuccessMessage:
		ProcessCatchSuccessMessage();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}

}

void ABattlePokeBallStateMachine::ProcessBallUseMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::PokeBallThrow;

		Timer = BallThrowTime;
		Canvas->SetCatchBallActive(true);
		BallThrowVelocity = UPokemonUtil::PixelVector(200, -200);
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallThrow(float _DeltaTime)
{
	BallThrowVelocity += UPokemonUtil::PixelVector(0, 250) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallThrowVelocity * _DeltaTime);

	if (Timer <= 0.0f)
	{
		if (true == Enemy->IsTrainer())
		{
			// Don't be a thief!
			// (트레이너에게 던지는 경우)
			// RED used\nPOKe BALL!
			// The TRAINER blocked the BALL!
			// 하고 플레이어 턴 종료됨

			//MsgBox->SetMessage(L"Ball missed!");
			//MsgBox->Write();
			State = ESubstate::End;
		}
		else
		{
			State = ESubstate::PokeBallPullInPokemon;
			Canvas->PlayCatchBallOpenAnimation();
			Timer = PullInTime;
		}
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlocked()
{
}

void ABattlePokeBallStateMachine::ProcessDontBeAThiefMessage()
{
}

void ABattlePokeBallStateMachine::ProcessPokeBallPullInPokemon()
{
	Canvas->LerpCatchPullInEnemyPokemon(Timer / PullInTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallClosing;
		Timer = ClosingTime;
		Canvas->PlayCatchBallCloseAnimation();
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallClosing()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallVerticalMove()
{
}

void ABattlePokeBallStateMachine::ProcessTestCatch()
{
}

void ABattlePokeBallStateMachine::ProcessCatchResultAnim()
{
}

void ABattlePokeBallStateMachine::ProcessCatchFailMessage()
{
}

void ABattlePokeBallStateMachine::ProcessCatchSuccessMessage()
{
	//Gotcha RATTATA\nwas caught!
}

