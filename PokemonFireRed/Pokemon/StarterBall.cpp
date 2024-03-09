﻿#include "StarterBall.h"
#include <EnginePlatform/EngineInput.h>
#include "PlayerData.h"
#include "EventManager.h"
#include "Player.h"
#include "DialogueWindow.h"

AStarterBall::AStarterBall()
{
}

AStarterBall::~AStarterBall()
{
}

void AStarterBall::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(ERenderingOrder::Lower);
	Renderer->SetImage(RN::MapBall);
	Renderer->SetTransform({ {0, 0}, {Global::FloatTileSize, Global::FloatTileSize} });

	Canvas = GetWorld()->SpawnActor<AStarterCanvas>();
	MsgBox = GetWorld()->SpawnActor<APokemonMsgBox>();

	MsgBox->SetBackgroundImage(RN::DialogueWindow);
	MsgBox->SetCoverImage(RN::DialogueWindowCover);
	MsgBox->SetTextColor(EFontColor::Gray);
	MsgBox->SetLineSpace(16);

	Canvas->SetActive(false);
	MsgBox->SetActive(false);
}

void AStarterBall::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	switch (State)
	{
	case AStarterBall::EState::None:
		CheckEventOccur();
		break;
	case AStarterBall::EState::NonEventMessage:
		ProcessNonEventMessage();
		break;
	case AStarterBall::EState::EventMessage1:
		ProcessEventMessage1();
		break;
	case AStarterBall::EState::EventMessage2:
		ProcessEventMessage2();
		break;
	case AStarterBall::EState::EventMessage3:
		ProcessEventMessage3();
		break;
	case AStarterBall::EState::Select:
		ProcessSelect();
		break;
	case AStarterBall::EState::SelectMessage1:
		ProcessSelectMessage1();
		break;
	case AStarterBall::EState::SelectMessage2:
		ProcessSelectMessage2();
		break;
	case AStarterBall::EState::End:
		break;
	default:
		break;
	}
}

bool AStarterBall::IsZClickEventOccur()
{
	if (false == UEngineInput::IsDown('Z'))
	{
		return false;
	}

	APlayer* Player = UEventManager::FindCurLevelTarget<APlayer>(EN::Player);
	FTileVector PlayerPoint = Player->GetPoint();
	FTileVector PlayerDirection = Player->GetDirection();
	FTileVector ThisPoint = FTileVector(GetActorLocation());
	bool IsStaring = (PlayerPoint + PlayerDirection) == ThisPoint;
	return IsStaring;
}

void AStarterBall::CheckEventOccur()
{
	if (true == IsZClickEventOccur())
	{
		// 스타팅 포켓몬 획득 이벤트가 아직 발생하지 않은 경우 
		if (false == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart))
		{
			// 대화창만 대충 띄운다.
			State = EState::NonEventMessage;
			MsgBox->SetActive(true);
			MsgBox->SetTextColor(EFontColor::Gray);
			MsgBox->SetMessage(L"Those are POKé BALLS.\nThey contain POKéMON!");
			MsgBox->Write();
			UEventManager::DeactivatePlayer();
			return;
		}
		// 스타팅 포켓몬을 획득해야 하는 경우
		else if ((true == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart))
			&& (false == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon)))
		{
			// 스타팅 포켓몬 선택 대화창을 띄운다.
			StateChangeToEventMessage1();
			UEventManager::DeactivatePlayer();
			return;
		}
		// 스타팅 포켓몬 획득 이후
		else
		{
			// 대화창만 대충 띄운다.
			State = EState::NonEventMessage;
			MsgBox->SetActive(true);
			MsgBox->SetTextColor(EFontColor::Gray);
			MsgBox->SetMessage(L"That's PROF. OAK's last POKéMON");
			MsgBox->Write();
			UEventManager::DeactivatePlayer();
			return;
		}
	}
}

void AStarterBall::ProcessNonEventMessage()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd && true == UEngineInput::IsDown('Z'))
	{
		State = EState::None;
		UEventManager::ActivatePlayer();
		MsgBox->SetActive(false);
	}
}

void AStarterBall::ProcessEventMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		MsgBox->ShowSkipArrow();
		State = EState::EventMessage2;
	}
}

void AStarterBall::ProcessEventMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		MsgBox->HideSkipArrow();
		StateChangeToEventMessage3();
	}
}

void AStarterBall::ProcessEventMessage3()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = EState::Select;
		Canvas->SetOptionBoxActive(true);
	}
}

void AStarterBall::ProcessSelect()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		if (Canvas->GetCursor() == 0)
		{
			State = EState::SelectMessage1;
			Canvas->SetActive(false);
			MsgBox->SetMessage(L"This POKeMON is really quite\nenergetic!");
			MsgBox->Write();
		}
		else
		{
			State = EState::None;
			UEventManager::ActivatePlayer();
			MsgBox->SetActive(false);
			Canvas->SetActive(false);
		}
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		State = EState::None;
		UEventManager::ActivatePlayer();
		MsgBox->SetActive(false);
		Canvas->SetActive(false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		Canvas->DecCursor();
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->IncCursor();
		return;
	}
}

void AStarterBall::ProcessSelectMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd && true == UEngineInput::IsDown('Z'))
	{
		State = EState::SelectMessage2;
		Renderer->SetActive(false);

		const FPokemonSpecies* Species = UPokemonDB::FindSpecies(PokemonId);
		MsgBox->SetTextColor(EFontColor::Gray);
		MsgBox->SetMessage(L"RED received the " + 
			UPokemonString::ToUpperW(Species->Name) + L"\nfrom PROF. OAK!");
		MsgBox->Write();
	}
}

void AStarterBall::ProcessSelectMessage2()
{
	// TODO: 라이벌이 포켓몬 고르는 이벤트 트리거
	// UEventManager::TriggerEvent();

	if (MsgBox->GetWriteState() == EWriteState::WriteEnd && true == UEngineInput::IsDown('Z'))
	{
		UPokemon Starter = UPokemon(PokemonId, 5);
		UPlayerData::AddPokemonToEntry(Starter);
		UPlayerData::Achieve(EAchievement::GetFirstPokemon);
		UEventManager::ActivatePlayer();
		MsgBox->SetActive(false);
		Destroy();
	}
}

void AStarterBall::StateChangeToEventMessage1()
{
	std::wstring Message;
	switch (PokemonId)
	{
	case EPokemonId::Bulbasaur:
		Message = L"I see! BULBASAUR is your choice.\nIt's very easy to raise.";
		break;
	case EPokemonId::Squirtle:
		Message = L"Hm! SQUIRTLE is your choice.\nIt's one worth raising.";
		break;
	case EPokemonId::Charmander:
		Message = L"Ah! CHARMANDER is your choice.\nYou should raise it patiently.";
		break;
	default:
		break;
	}
	MsgBox->SetActive(true);
	MsgBox->SetTextColor(EFontColor::Blue);
	MsgBox->SetMessage(Message);
	MsgBox->Write();

	Canvas->SetActive(true);
	Canvas->SetOptionBoxActive(false);
	Canvas->SetPokemon(PokemonId);

	State = EState::EventMessage1;
}

void AStarterBall::StateChangeToEventMessage3()
{
	std::wstring Message;
	switch (PokemonId)
	{
	case EPokemonId::Bulbasaur:
		Message = L"So, RED, you want to go with\nthe GRASS POKéMON BULBASAUR?";
		break;
	case EPokemonId::Squirtle:
		Message = L"So, RED, you've decided on the\nWATER POKéMON SQUIRTLE?";
		break;
	case EPokemonId::Charmander:
		Message = L"So, RED, you're claiming the\nFIRE POKéMON CHARMANDER?";
		break;
	default:
		break;
	}

	MsgBox->SetMessage(Message);
	MsgBox->Write();

	State = EState::EventMessage3;
}
