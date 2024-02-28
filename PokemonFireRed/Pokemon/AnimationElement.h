#pragma once
#include "UIElement.h"

class ACanvas;

class AAnimationElement : public AUIElement
{
	friend ACanvas;
public:
	// constructor destructor
	AAnimationElement();
	~AAnimationElement();

	// delete Function
	AAnimationElement(const AAnimationElement& _Other) = delete;
	AAnimationElement(AAnimationElement&& _Other) noexcept = delete;
	AAnimationElement& operator=(const AAnimationElement& _Other) = delete;
	AAnimationElement& operator=(AAnimationElement&& _Other) noexcept = delete;

	void CreateAnimation(std::string_view _AnimName, std::string_view _ImageName, int _Start, int _End, float _Interval, bool _Loop);
	void SetAnimation(std::string_view _AnimName);

protected:

private:
	UImageRenderer* Renderer = nullptr;
};

