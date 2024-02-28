#include "FadeLevelChanger.h"

AFadeLevelChanger::AFadeLevelChanger() 
{
}

AFadeLevelChanger::~AFadeLevelChanger() 
{
}

void AFadeLevelChanger::Update(std::string_view _TargetLevelName, float _FadeInTime, float _FadeOutTime)
{
	FadeInTime = _FadeInTime;
	FadeOutTime = _FadeOutTime;

	if (TargetLevelName != _TargetLevelName)
	{
		UnregisterEvent();
	}

	TargetLevelName = _TargetLevelName;
	RegisterPredefinedEvent();
}

void AFadeLevelChanger::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(false)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::ChangeLevel(TargetLevelName)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::End(false)
	);
}
