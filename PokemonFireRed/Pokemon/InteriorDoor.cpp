#include "InteriorDoor.h"

AInteriorDoor::AInteriorDoor()
{
}

AInteriorDoor::~AInteriorDoor()
{
}

void AInteriorDoor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	if (true == IsRegistered)
	{
		UEventManager::UnregisterEvent(this, Cond);
	}

	IsRegistered = true;

	Cond = UEventCondition(EEventTriggerAction::ArrowClick);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::PlayAnimation(Global::Player, Global::Player + "Idle" + TargetDirection.ToDirectionString())
		>> ES::PlaySE(RN::SEExitMap)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::Player, TargetPoint - TargetDirection)
		>> ES::ChangeDirection(TargetMapName, Global::Player, TargetDirection)
		>> ES::CameraFocus(Global::Player)
		>> ES::HideActor(Global::Player)
		>> ES::ShowMapName(TargetMapNameText)
		>> ES::FadeIn(0.75f, EFadeType::HCurtain)
		>> ES::Wait(0.75f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorOpen")
		>> ES::ShowActor(Global::Player)
		>> ES::Move(Global::Player, { TargetDirection }, 1.8f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorClose")
		>> ES::End(true)
	);
}