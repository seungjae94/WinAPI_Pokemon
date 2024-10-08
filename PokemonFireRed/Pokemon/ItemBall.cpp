#include "ItemBall.h"
#include "EventMacros.h"

AItemBall::AItemBall() 
{
}

AItemBall::~AItemBall() 
{
}

void AItemBall::SetItem(EItemId _ItemId, int _Count)
{
	ItemId = _ItemId;
	Count = _Count;
}

void AItemBall::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	const FItem* Item = UGameData::FindItem(ItemId);

	RegisterEvent(
		EEventTriggerAction::ZClick,
		SKIP_CHECK,
		ES::Start(true)
		>> ES::PlaySE(RN::SEItemObtained, Global::LevelUpFanfareTime)
		>> ES::Chat({ L"RED found " + std::to_wstring(Count) + L" " + Item->Name + L"!"}, EFontColor::Gray, 16)
		>> ES::GainItem(ItemId, Count)
		>> ES::Destroy(this)
		>> ES::End(true)
	);
}

