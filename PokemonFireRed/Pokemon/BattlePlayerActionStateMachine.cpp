#include "BattlePlayerActionStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"

ABattlePlayerActionStateMachine::ABattlePlayerActionStateMachine()
{
}

ABattlePlayerActionStateMachine::~ABattlePlayerActionStateMachine()
{
}

void ABattlePlayerActionStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattlePlayerActionStateMachine::ESubstate::None:
		break;
	case ABattlePlayerActionStateMachine::ESubstate::Select:
		ProcessSelect(_DeltaTime);
		break;
	case ABattlePlayerActionStateMachine::ESubstate::ShowEscapeSuccessMsg:
		ProcessShowEscapeSuccessMsg(_DeltaTime);
		break;
	case ABattlePlayerActionStateMachine::ESubstate::ShowEscapeFailMsg:
		ProcessShowEscapeFailMsg(_DeltaTime);
		break;
	case ABattlePlayerActionStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattlePlayerActionStateMachine::ProcessSelect(float _DeltaTime)
{
	int Cursor = Canvas->GetActionCursor();

	if (true == UEngineInput::IsDown('Z'))
	{
		switch (Cursor)
		{
		case Fight:
			break;
		case Bag:
			break;
		case Pokemon:
			break;
		case Run:
		{
			bool RunResult = CalcRunResult();

			if (true == RunResult)
			{
				State = ESubstate::ShowEscapeSuccessMsg;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Got away safely!");
			}
			else
			{
				State = ESubstate::ShowEscapeFailMsg;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Can't escape!");
			}
		}
		break;
		default:
			break;
		}
	}


	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		if (Cursor % 2 == 1)
		{
			Canvas->SetActionCursor(Cursor - 1);
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		if (Cursor % 2 == 0)
		{
			Canvas->SetActionCursor(Cursor + 1);
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		if (Cursor >= 2)
		{
			Canvas->SetActionCursor(Cursor - 2);
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		if (Cursor < 2)
		{
			Canvas->SetActionCursor(Cursor + 2);
			return;
		}
	}
}

bool ABattlePlayerActionStateMachine::CalcRunResult()
{
	const UPokemon* PlayerPokemon = &GetCurPlayerPokemon();

	int PSpeed = PlayerPokemon->GetSpeed();
	int ESpeed = EnemyPokemon->GetSpeed();

	if (PSpeed >= ESpeed)
	{
		return true;
	}

	int RandomNumber = UPokemonMath::RandomInt(0, 255);

	int RHS = UPokemonMath::Floor(PSpeed * 128.0f / ESpeed);
	RHS += 30 * RunAttemptCount;
	RHS = UPokemonMath::Mod(RHS, 256);
	return RandomNumber < RHS;
}

void ABattlePlayerActionStateMachine::ProcessShowEscapeSuccessMsg(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ActionResult = EBattlePlayerAction::EscapeSuccess;
		State = ESubstate::End;
	}
}

void ABattlePlayerActionStateMachine::ProcessShowEscapeFailMsg(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ActionResult = EBattlePlayerAction::EscapeFail;
		State = ESubstate::End;
	}
}