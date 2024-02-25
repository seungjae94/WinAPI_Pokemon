#pragma once
#include <EngineCore/Actor.h>
#include "AllUIElements.h"

class UEventProcessor;

class APage : public AActor
{
	friend UEventProcessor;
public:
	// constructor destructor
	APage();
	~APage();

	// delete Function
	APage(const APage& _Other) = delete;
	APage(APage&& _Other) noexcept = delete;
	APage& operator=(const APage& _Other) = delete;
	APage& operator=(APage&& _Other) noexcept = delete;

	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;

	AText* CreateText(
		UImageRenderer* _Container,
		std::wstring _Text,
		EPivotType _PivotType = EPivotType::LeftTop,
		EAlignType _AlignType = EAlignType::Left,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0,
		EFontColor _Color = EFontColor::White,
		EFontSize _Size = EFontSize::Normal
	);

	AScrollBar* CreateScrollBar(
		UImageRenderer* _Container,
		EScrollType _ScrollType,
		int _CurValue,
		int _MaxValue,
		EPivotType _PivotType = EPivotType::LeftTop,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0
	);

	ACursor* CreateCursor(
		UImageRenderer* _Container,
		int _Cursor,
		int _OptionCount,
		EPivotType _PivotType = EPivotType::LeftTop,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0,
		int _PixelGap = 16
	);

	APokemonElement* CreatePokemonIcon(
		UImageRenderer* _Container,
		EPivotType _PivotType = EPivotType::LeftTop,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0
	);

	AImageElement* CreateImageElement(
		UImageRenderer* _Container,
		EPivotType _PivotType = EPivotType::LeftTop,
		int _RelativePixelX = 0,
		int _RelativePixelY = 0
	);

protected:
	virtual void Sync(APage* _Other) {}
	
	void ContainerElementSyncOff()
	{
		ContainerElementSync = false;
	}

	void Tick(float _DeltaTime) override;
private:
	bool ContainerElementSync = true;
	std::map<UImageRenderer*, std::list<AUIElement*>> Elements;
};

