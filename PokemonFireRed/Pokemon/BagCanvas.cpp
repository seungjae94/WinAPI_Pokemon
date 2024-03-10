#include "BagCanvas.h"
#include "PlayerData.h"
#include "Item.h"

ABagCanvas::ABagCanvas()
{
}

ABagCanvas::~ABagCanvas()
{
}

void ABagCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);

	//LeftArrow = CreateImageElement(Background, ERenderingOrder::UI0, EPivotType::LeftTop, 3, 65);
	//LeftArrow->SetImage(RN::BagArrowLeft);
	//LeftArrow->CreateAnimation(Global::BagArrowLeft, 0, 1, 0.1f, true);
	//LeftArrow->ChangeAnimation(Global::BagArrowLeft);
	//
	//RightArrow = CreateImageElement(Background, ERenderingOrder::UI0, EPivotType::LeftTop, 70, 65);
	//RightArrow->SetImage(RN::BagArrowLeft);
	//RightArrow->CreateAnimation(Global::BagArrowLeft, 0, 1, 0.1f, true);
	//RightArrow->ChangeAnimation(Global::BagArrowLeft);

	ItemImage = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 4, -8);

	ItemExplain = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 40, -33, EAlignType::Left, EFontColor::White);
	
	TargetCursor = CreateCursor(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 90, 12);
	TargetCursor->SetOptionCount(6);

	const int RowGap = 16;
	for (int i = 0; i < 6; ++i)
	{
		AText* ItemName = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 97, 21 + RowGap * i, EAlignType::Left, EFontColor::Black3);
		AText* ItemCross = CreateText(Background, ERenderingOrder::UI1, EPivotType::RightTop, -35, 20 + RowGap * i, EAlignType::Right, EFontColor::Black3);
		AText* ItemCount = CreateText(Background, ERenderingOrder::UI1, EPivotType::RightTop, -19, 21 + RowGap * i, EAlignType::Right, EFontColor::Black3, EFontSize::Mini);

		ItemNames.push_back(ItemName);
		ItemCrosses.push_back(ItemCross);
		ItemCounts.push_back(ItemCount);
	}
}


void ABagCanvas::SetBackground(std::string_view _ImageName)
{
	Background->SetImage(_ImageName);
}

void ABagCanvas::SetItemImage(std::string_view _ImageName)
{
	ItemImage->SetImage(_ImageName);
}

void ABagCanvas::SetItemExplain(std::wstring_view _Explain)
{
	ItemExplain->SetText(_Explain);
}

void ABagCanvas::SetTargetCursor(int _Cursor)
{
	TargetCursor->SetCursor(_Cursor);
}

void ABagCanvas::RefreshList(const std::list<FInventoryRecord>& _Records)
{
	int Index = 0;
	for (const FInventoryRecord& Record : _Records)
	{
		const FItem* Item = UGameDB::FindItem(Record.Id);
		ItemNames[Index]->SetText(Item->Name);
		ItemCrosses[Index]->SetText(L"x");
		ItemCounts[Index]->SetText(std::to_wstring(Record.Count));
		++Index;
	}

	if (Index >= 6)
	{
		return;
	}

	ItemNames[Index]->SetText(L"CANCEL");
	ItemCrosses[Index]->SetText(L"");
	ItemCounts[Index]->SetText(L"");

	for (int i = Index + 1; i < 6; ++i)
	{
		ItemNames[Index]->SetText(L"");
		ItemCrosses[Index]->SetText(L"");
		ItemCounts[Index]->SetText(L"");
	}
}