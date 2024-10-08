#pragma once
#include <EngineCore/Actor.h>
#include "AllUIElements.h"
#include "UIParent.h"

class UEventProcessor;

class ACanvas : public AUIParent
{
	friend UEventProcessor;
public:
	// constructor destructor
	ACanvas();
	~ACanvas();

	// delete Function
	ACanvas(const ACanvas& _Other) = delete;
	ACanvas(ACanvas&& _Other) noexcept = delete;
	ACanvas& operator=(const ACanvas& _Other) = delete;
	ACanvas& operator=(ACanvas&& _Other) noexcept = delete;

	AText* CreateText(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		EAlignType _AlignType = EAlignType::Left,
		EFontColor _Color = EFontColor::White,
		EFontSize _Size = EFontSize::Normal
	);

	ABar* CreateBar(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		EBarType _BarType
	);

	ACursor* CreateCursor(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		std::string _ImageName = RN::BlackCursor,
		int _PixelGap = 16
	);

	AImageElement* CreateImageElement(
		AUIParent* _Parent,
		ERenderingOrder _Order,
		EPivotType _PivotType,
		int _RelativePixelX,
		int _RelativePixelY,
		EImageElementType _Type = EImageElementType::Image
	);

	FTransform GetUITransform() override;

	// 에러 해결용 임시 함수
	virtual void Sync(ACanvas* _Canvas) {};

protected:
private:
};

