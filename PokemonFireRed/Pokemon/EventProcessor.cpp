#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>
#include "EventStream.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "Player.h"
#include "BlackScreen.h"
#include "DialogueWindow.h"

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

void UEventProcessor::RegisterStream(const UEventCondition& _Condition, UEventStream _Stream)
{
	AllStreams[_Condition] = _Stream;
}

bool UEventProcessor::TryRun(EEventTriggerAction _TriggerAction)
{
	for (std::pair<const UEventCondition, UEventStream>& Pair : AllStreams)
	{
		const UEventCondition& Condition = Pair.first;

		// 조건을 만족하고 현재 실행중이 아닐 때만 실행
		bool CheckResult = Condition.Check(_TriggerAction);
		if (true == CheckResult && false == IsRunningValue)
		{
			CurCommandIndex = 0;
			CurStream = &Pair.second;
			if (true == CurStream->DeactivatePlayer)
			{
				DeactivatePlayerControl();
			}
			IsRunningValue = true;		// 실행
			CurIndexOfTypeMap.clear();
			return true;
		}
	}

	return false;
}

void UEventProcessor::EndRun()
{
	if (true == CurStream->ActivatePlayer)
	{
		ActivatePlayerControl();
	}

	UEventManager::PlayerEventProcessingOff();
	IsRunningValue = false;
	CurIndexOfTypeMap.clear();
}

void UEventProcessor::Tick(float _DeltaTime)
{
	if (false == IsRunningValue)
	{
		return;
	}

	EEventType CurEventType = CurStream->EventTypeList[CurCommandIndex];

	if (CurEventType == EEventType::End)
	{
		EndRun();
		return;
	}

	bool ProcessingResult = false;
	switch (CurEventType)
	{
	case EEventType::MoveActor:
		ProcessingResult = ProcessMoveActor();
		break;
	case EEventType::FadeIn:
		ProcessingResult = ProcessFadeIn();
		break;
	case EEventType::FadeOut:
		ProcessingResult = ProcessFadeOut();
		break;
	case EEventType::Wait:
		ProcessingResult = ProcessWait();
		break;
	case EEventType::Chat:
		ProcessingResult = ProcessChat();
		break;
	case EEventType::ChangeLevel:
		ProcessingResult = ProcessChangeLevel();
		break;
	case EEventType::ChangePoint:
		ProcessingResult = ProcessChangePoint();
		break;
	case EEventType::ChangeDirection:
		ProcessingResult = ProcessChangeDirection();
		break;
	default:
		break;
	}

	if (true == ProcessingResult)
	{
		IncCurIndexOfType(CurEventType);
		CurCommandIndex++;
	}
}

// 이벤트 명령 처리 함수

void UEventProcessor::ActivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	CurPlayer->StateChange(EPlayerState::Idle);
}

void UEventProcessor::DeactivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	CurPlayer->StateChange(EPlayerState::OutOfControl);
}

bool UEventProcessor::ProcessMoveActor()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::MoveActor);
	ES::MoveActor& Data = CurStream->MoveActorDataSet[CurIndexOfType];

	std::string MapName = UEngineString::ToUpper(Data.MapName);
	std::string TargetName = UEngineString::ToUpper(Data.TargetName);

	if (Data.Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동 경로의 크기가 0 이하입니다.");
		return false;
	}

	AEventTarget* Target = UEventManager::FindTarget(MapName, TargetName);
	if (nullptr == Target)
	{
		MsgBoxAssert(MapName + ":" + TargetName + "는 존재하지 않는 이벤트 타겟입니다.존재하지 않는 이벤트 타겟을 이동시키려고 했습니다.");
		return false;
	}

	// 이동 이벤트 시작
	if (MoveActorPathIndex == -1)
	{
		MoveActorMoveTime = 1.0f / Data.MoveSpeed;
		MoveActorTimer = 0.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
		Target->ChangeAnimation(Target->GetMoveState(), Target->GetDirection());
	}

	if (MoveActorTimer > 0.0f)
	{
		MoveActorTimer -= DeltaTime;

		float t = (MoveActorMoveTime - MoveActorTimer) / MoveActorMoveTime;
		FVector TargetPos = UPokemonMath::Lerp(MoveActorPrevPoint.ToFVector(), MoveActorNextPoint.ToFVector(), t);
		Target->SetActorLocation(TargetPos);
		Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HALF_SCREEN);
	}
	else if (MoveActorPathIndex + 1 >= Data.Path.size())
	{
		// 이동 종료
		PostProcessMoveActor(Target);
		return true;
	}
	else
	{
		UEventManager::SetPoint(MapName, TargetName, FTileVector(Target->GetActorLocation()));

		MoveActorPrevPoint = Target->GetPoint();
		MoveActorNextPoint = MoveActorPrevPoint + Data.Path[MoveActorPathIndex + 1];
		MoveActorTimer = MoveActorMoveTime;

		if (Data.Path[MoveActorPathIndex + 1] == FTileVector::Zero)
		{
			MsgBoxAssert("MoveActor 함수에서 Path 값이 FTileVector::Zero 입니다.");
			return false;
		}

		if (Target->GetDirection() != Data.Path[MoveActorPathIndex + 1])
		{
			Target->SetDirection(Data.Path[MoveActorPathIndex + 1]);
			Target->ChangeAnimation(Target->GetMoveState(), Target->GetDirection());
		}
		MoveActorPathIndex++;
	}
	return false;
}

void UEventProcessor::PostProcessMoveActor(AEventTarget* _Target)
{
	MoveActorPrevPoint = FTileVector::Zero;
	MoveActorNextPoint = FTileVector::Zero;
	MoveActorPathIndex = -1;		// -1은 첫 번째 틱임을 의미한다.
	MoveActorMoveTime = 0.0f;
	MoveActorTimer = 0.0f;
	_Target->SetMoveState(ETargetMoveState::Idle);
	_Target->ChangeAnimation(_Target->GetMoveState(), _Target->GetDirection());
}

bool UEventProcessor::ProcessFadeIn()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();
	std::string CurLevelName = UEventManager::GetCurLevelName();
	ABlackScreen* BlackScreen = UEventManager::GetBlackScreen(CurLevelName);

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeIn);
	ES::FadeIn& Data = CurStream->FadeInDataSet[CurIndexOfType];

	if (true == IsBegin)
	{
		Timer = Data.Time;
		IsBegin = false;
		BlackScreen->SetActiveRenderer(true);
		BlackScreen->SetAlpha(1.0f);
	}

	if (Timer <= 0.0f)
	{
		IsBegin = true;
		BlackScreen->SetActiveRenderer(false);
		return true;
	}

	Timer -= DeltaTime;
	BlackScreen->SetAlpha(Timer / Data.Time);
	return false;
}

bool UEventProcessor::ProcessFadeOut()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();
	std::string CurLevelName = UEventManager::GetCurLevelName();
	ABlackScreen* BlackScreen = UEventManager::GetBlackScreen(CurLevelName);

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeOut);
	ES::FadeOut& Data = CurStream->FadeOutDataSet[CurIndexOfType];

	if (true == IsBegin)
	{
		Timer = Data.Time;
		IsBegin = false;
		BlackScreen->SetActiveRenderer(true);
		BlackScreen->SetAlpha(0.0f);
	}

	if (Timer <= 0.0f)
	{
		IsBegin = true;
		BlackScreen->SetAlpha(1.0f);
		return true;
	}

	Timer -= DeltaTime;
	BlackScreen->SetAlpha((Data.Time - Timer) / Data.Time);
	return false;

}

bool UEventProcessor::ProcessWait()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();

	int CurIndexOfType = GetCurIndexOfType(EEventType::Wait);
	ES::Wait& Data = CurStream->WaitDataSet[CurIndexOfType];

	if (true == IsBegin)
	{
		Timer = Data.Time;
		IsBegin = false;
	}

	if (Timer <= 0.0f)
	{
		IsBegin = true;
		return true;
	}

	Timer -= DeltaTime;

	return false;
}

bool UEventProcessor::ProcessChat()
{
	ADialogueWindow* CurDialogueWindow = UEventManager::GetCurDialogueWindow();

	int CurIndexOfType = GetCurIndexOfType(EEventType::Chat);
	ES::Chat& Data = CurStream->ChatDataSet[CurIndexOfType];

	EDialogueWindowState State = CurDialogueWindow->GetState();

	if (State == EDialogueWindowState::End)
	{
		CurDialogueWindow->SetState(EDialogueWindowState::Hide);
		return true;
	}

	if (State == EDialogueWindowState::Show)
	{
		return false;
	}

	if (false == CurDialogueWindow->IsActive())
	{
		CurDialogueWindow->ActiveOn();
		CurDialogueWindow->AllRenderersActiveOn();
		CurDialogueWindow->SetDialogue(Data.Dialogue, Data.Color, Data.LineSpace, Data.IsSequential);
		return false;
	}

	return false;
}

bool UEventProcessor::ProcessChangeLevel()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangeLevel);
	ES::ChangeLevel& Data = CurStream->ChangeLevelDataSet[CurIndexOfType];
	UEventManager::SetLevel(Data.LevelName);
	DeactivatePlayerControl();
	return true;
}

bool UEventProcessor::ProcessChangePoint()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangePoint);
	ES::ChangePoint& Data = CurStream->ChangePointDataSet[CurIndexOfType];
	UEventManager::SetPoint(Data.LevelName, Data.TargetName, Data.Point);
	return true;
}

bool UEventProcessor::ProcessChangeDirection()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangeDirection);
	ES::ChangeDirection& Data = CurStream->ChangeDirectionDataSet[CurIndexOfType];
	UEventManager::SetDirection(Data.LevelName, Data.TargetName, Data.Direction);
	return true;
}