﻿#include "FinishBattleStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "Battler.h"
#include "PlayerData.h"

AFinishBattleStateMachine::AFinishBattleStateMachine() 
{
}

AFinishBattleStateMachine::~AFinishBattleStateMachine() 
{
}

void AFinishBattleStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy, EBattleEndReason _BattleEndReason)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Player = _Player;
	Enemy = _Enemy;
	Reason = _BattleEndReason;

	switch (_BattleEndReason)
	{
	case EBattleEndReason::None:
		break;
	case EBattleEndReason::WinToWild:
		State = ESubstate::End;
		break;
	case EBattleEndReason::WinToTrainer:
		State = ESubstate::PlayerDefeated1;
		MsgBox->SetMessage(L"Player defeated\n" + Enemy->GetTrainerNameW() + L"!");
		MsgBox->Write();
		break;
	case EBattleEndReason::LoseToWild:
		State = ESubstate::OutOfPokemonMessage1;
		MsgBox->SetMessage(L"RED is out of\nusable POKéMON!");
		MsgBox->Write();
		break;
	case EBattleEndReason::LoseToTrainer:
		State = ESubstate::OutOfPokemonMessage1;
		MsgBox->SetMessage(L"RED is out of\nusable POKéMON!");
		MsgBox->Write();
		break;
	default:
		break;
	}
}

void AFinishBattleStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case AFinishBattleStateMachine::ESubstate::None:
		break;
	case AFinishBattleStateMachine::ESubstate::OutOfPokemonMessage1:
		ProcessOutOfPokemonMessage1();
		break;
	case AFinishBattleStateMachine::ESubstate::OutOfPokemonMessage2:
		ProcessOutOfPokemonMessage2();
		break;
	case AFinishBattleStateMachine::ESubstate::PanicAndLost1:
		ProcessPanicAndLost1();
		break;
	case AFinishBattleStateMachine::ESubstate::PanicAndLost2:
		ProcessPanicAndLost2();
		break;
	case AFinishBattleStateMachine::ESubstate::ManyDots1:
		ProcessManyDots1();
		break;
	case AFinishBattleStateMachine::ESubstate::ManyDots2:
		ProcessManyDots2();
		break;
	case AFinishBattleStateMachine::ESubstate::WhitedOut1:
		ProcessWhitedOut1();
		break;
	case AFinishBattleStateMachine::ESubstate::WhitedOut2:
		ProcessWhitedOut2();
		break;
	case AFinishBattleStateMachine::ESubstate::PlayerLostAgainst1:
		ProcessPlayerLostAgainst1();
		break;
	case AFinishBattleStateMachine::ESubstate::PlayerLostAgainst2:
		ProcessPlayerLostAgainst2();
		break;
	case AFinishBattleStateMachine::ESubstate::PaidAsThePrizeMoney1:
		ProcessPaidAsThePrizeMoney1();
		break;
	case AFinishBattleStateMachine::ESubstate::PaidAsThePrizeMoney2:
		ProcessPaidAsThePrizeMoney2();
		break;
	case AFinishBattleStateMachine::ESubstate::PlayerDefeated1:
		ProcessPlayerDefeated1();
		break;
	case AFinishBattleStateMachine::ESubstate::PlayerDefeated2:
		ProcessPlayerDefeated2();
		break;
	case AFinishBattleStateMachine::ESubstate::EnemyBattlerMove:
		ProcessEnemyBattlerMove();
		break;
	case AFinishBattleStateMachine::ESubstate::EnemyBattlerMessage:
		break;
	case AFinishBattleStateMachine::ESubstate::GotMoneyForWining1:
		ProcessGotMoneyForWining1();
		break;
	case AFinishBattleStateMachine::ESubstate::GotMoneyForWining2:
		ProcessGotMoneyForWining2();
		break;
	case AFinishBattleStateMachine::ESubstate::End:
		break;
	default:
		break;
	}

}

void AFinishBattleStateMachine::ProcessOutOfPokemonMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::OutOfPokemonMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void AFinishBattleStateMachine::ProcessOutOfPokemonMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		if (Reason == EBattleEndReason::LoseToWild)
		{
			State = ESubstate::PanicAndLost1;
			int LostMoney = CalcLostMoney();
			UPlayerData::LostMoney(LostMoney);
			MsgBox->HideSkipArrow();
			MsgBox->SetMessage(L"RED panicked and lost " + std::to_wstring(LostMoney) + L"G");
			MsgBox->Write();
		}
		else if (Reason == EBattleEndReason::LoseToTrainer)
		{
			State = ESubstate::PlayerLostAgainst1;
			MsgBox->HideSkipArrow();
			MsgBox->SetMessage(L"Player lost against\n" + Enemy->GetTrainerNameW() + L"!");
			MsgBox->Write();
		}
	}
}

void AFinishBattleStateMachine::ProcessPanicAndLost1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::PanicAndLost2;
		MsgBox->ShowSkipArrow();
	}
}

void AFinishBattleStateMachine::ProcessPanicAndLost2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::ManyDots1;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"… … … …");
		MsgBox->Write();
	}
}

void AFinishBattleStateMachine::ProcessManyDots1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::ManyDots2;
		MsgBox->ShowSkipArrow();
	}
}

void AFinishBattleStateMachine::ProcessManyDots2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::WhitedOut1;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"RED whited out!");
		MsgBox->Write();
	}
}

void AFinishBattleStateMachine::ProcessWhitedOut1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::WhitedOut2;
	}
}

void AFinishBattleStateMachine::ProcessWhitedOut2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::End;
	}
}

void AFinishBattleStateMachine::ProcessPlayerLostAgainst1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::PlayerLostAgainst2;
	}
}

void AFinishBattleStateMachine::ProcessPlayerLostAgainst2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::PaidAsThePrizeMoney1;
		int LostMoney = CalcLostMoney();
		UPlayerData::LostMoney(LostMoney);
		MsgBox->SetMessage(L"RED paid " + std::to_wstring(LostMoney) + L"G as the prize\nmoney…");
		MsgBox->Write();
	}
}

void AFinishBattleStateMachine::ProcessPaidAsThePrizeMoney1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::PaidAsThePrizeMoney2;
		MsgBox->ShowSkipArrow();
	}
}

void AFinishBattleStateMachine::ProcessPaidAsThePrizeMoney2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::ManyDots1;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"… … … …");
		MsgBox->Write();
	}
}

void AFinishBattleStateMachine::ProcessPlayerDefeated1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::PlayerDefeated2;
		MsgBox->ShowSkipArrow();
	}
}

void AFinishBattleStateMachine::ProcessPlayerDefeated2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::EnemyBattlerMove;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"");
		Timer = EnemyBattlerMoveTime;
	}
}

void AFinishBattleStateMachine::ProcessEnemyBattlerMove()
{
	Canvas->LerpShowEnemyBattler(Timer / EnemyBattlerMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::TestEnemyBattlerMessage;
	}
}

void AFinishBattleStateMachine::ProcessEnemyBattlerMessage()
{
}

void AFinishBattleStateMachine::ProcessGotMoneyForWining1()
{
}

void AFinishBattleStateMachine::ProcessGotMoneyForWining2()
{
}

int AFinishBattleStateMachine::CalcMaxLevel()
{
	int MaxLevel = 0;
	for (int i = 0; i < Player->GetEntrySize(); ++i)
	{
		MaxLevel = UPokemonMath::Max(MaxLevel, Player->GetLevel(i));
	}
	return MaxLevel;
}

int AFinishBattleStateMachine::CalcLostMoney()
{
	int MaxLevel = CalcMaxLevel();
	int LostMoney = UPokemonMath::Min(MaxLevel * 8, UPlayerData::GetMoney());
	return LostMoney;
}

int AFinishBattleStateMachine::CalcPrizeMoney()
{
	int MaxLevel = CalcMaxLevel();
	return MaxLevel * 100;
}