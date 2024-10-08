#include "DialogueActor.h"
#include "EventManager.h"
#include "EventStream.h"
#include "Text.h"
#include "PlayerCharacter.h"
#include "PokemonLevel.h"
#include "EventMacros.h"

ADialogueActor::ADialogueActor()
{
}

ADialogueActor::~ADialogueActor()
{
}

void ADialogueActor::BeginPlay()
{
	AEventTrigger::BeginPlay();
}

void ADialogueActor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	if (false == Rotatable)
	{
		RegisterEvent(
			EEventTriggerAction::ZClick,
			SKIP_CHECK,
			ES::Start(true)
			>> ES::Chat(Dialogue, TextColor, 16, true)
			>> ES::End(true)
		);
		return;
	}

	RegisterEvent(
		EEventTriggerAction::ZClick,
		SKIP_CHECK,
		ES::Start(true)
		>> ES::StarePlayer(GetName())
		>> ES::Chat(Dialogue, TextColor, 16, true)
		>> ES::End(true)
	);
}

ADialogueActor* ADialogueActor::GenerateObject(UPokemonLevel* _Level, std::string_view _Name, const FTileVector _Point, EFontColor _TextColor, const std::vector<std::wstring>& _Dialogue)
{
	UEventTargetSetting DAInit;
	DAInit.SetName(_Name);
	DAInit.SetPoint(_Point);

	ADialogueActor* DA = _Level->SpawnEventTrigger<ADialogueActor>(DAInit);
	DA->SetTextColor(_TextColor);
	DA->SetDialogue(_Dialogue);
	DA->RegisterPredefinedEvent();

	return DA;
}
