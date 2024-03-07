#pragma once
#include "SceneComponent.h"
#include <EnginePlatform\WindowImage.h>
#include <map>

enum class EImageSortType
{
	Center,
	Left,
};

class UAnimationInfo
{
public:
	UWindowImage* Image = nullptr;
	std::string Name;
	int CurFrame = 0;
	float CurTime = 0.0f;
	bool Loop = false;					// 애니메이션 반복 여부		
	bool IsEnd = false;					// 애니메이션 종료 여부
	std::vector<float> Times;			// Times[CurFrame]: 애니메이션 프레임의 시간 길이
	std::vector<int> Indexs;			// Indexs[CurFrame]: 애니메이션 프레임에 대응하는 이미지 인덱스

	int Update(float _DeltaTime);
};

class AActor;
class UWindowImage;

class UImageRenderer : public USceneComponent
{
	friend AActor;
public:
	UImageRenderer();
	~UImageRenderer();

	UImageRenderer(const UImageRenderer& _Other) = delete;
	UImageRenderer(UImageRenderer&& _Other) noexcept = delete;
	UImageRenderer& operator=(const UImageRenderer& _Other) = delete;
	UImageRenderer& operator=(UImageRenderer&& _Other) noexcept = delete;

	void SetOrder(int _Order) override;
	void Render(float _DeltaTime);
	void SetImage(std::string_view _Name, int _InfoIndex = 0);

	void SetImageIndex(int _InfoIndex)
	{
		InfoIndex = _InfoIndex;
	}

	void SetTransform(const FTransform& _Value)
	{
		USceneComponent::SetTransform(_Value);
	}

	void SetImageCuttingTransform(const FTransform& _Value)
	{
		ImageCuttingTransformSetting = true;
		ImageCuttingTransform = _Value;
	}

	FTransform GetImageCuttingTransform() const
	{
		return ImageCuttingTransform;
	}

	void CreateAnimation(
		std::string_view _AnimationName,
		std::string_view _ImageName,
		int _Start,
		int _End,
		float _Inter,
		bool _Loop = true
	);

	void CreateAnimation(
		std::string_view _AnimationName,
		std::string_view _ImageName,
		std::vector<int> _Indexs,
		float _Inter,
		bool _Loop = true
	);

	void CreateAnimation(
		std::string_view _AnimationName,
		std::string_view _ImageName,
		std::vector<int> _Indexs,
		std::vector<float> _Inters,
		bool _Loop = true
	);

	void ChangeAnimation(
		std::string_view _AnimationName, 
		bool _IsForce = false, 
		int _StartIndex = 0, 
		float _Time = -1.0f
	);
	
	void AnimationReset();

	void SetAngle(float _Angle)
	{
		Angle = _Angle;
	}

	void SetTransColor(Color8Bit _Color)
	{
		TransColor = _Color;
	}

	void SetAlpha(float _Alpha)
	{
		if (0.0f >= _Alpha)
		{
			_Alpha = 0.0f;
		}
		
		if (1.0f <= _Alpha)
		{
			_Alpha = 1.0f;
		}

		TransColor.A = static_cast<char>(_Alpha * 255.0f);
	}

	UWindowImage* GetImage() const
	{
		return Image;
	}

	void CameraEffectOff()
	{
		CameraEffect = false;
	}

	bool IsCurAnimationEnd() const
	{
		return CurAnimation->IsEnd;
	}

	int GetCurAnimationFrame() const
	{
		return CurAnimation->CurFrame;
	}

	int GetCurAnimationImageFrame() const
	{
		const std::vector<int>& Indexs = CurAnimation->Indexs;
		return Indexs[CurAnimation->CurFrame];
	}

	float GetCurAnimationTime() const
	{
		return CurAnimation->CurTime;
	}

	UAnimationInfo* GetCurAnimation() const
	{
		return CurAnimation;
	}

	void TextRender(float _DeltaTime);

	void ImageRender(float _DeltaTime);

	void SetText(std::string_view _Text)
	{
		Text = _Text;
	}
	void SetFont(std::string_view _Font)
	{
		Font = _Font;
	}

	void SetTextSize(float _Value)
	{
		Size = _Value;
	}
	
	void SetTextColor(Color8Bit _Color, Color8Bit _Color2 = Color8Bit::White)
	{
		TextColor = _Color;
		TextColor2 = _Color2;
	}

	void SetCameraRatio(float _Ratio)
	{
		CameraRatio = _Ratio;
	}

	FTransform GetRenderTransForm();

	void SetTextEffect(int _Effect = 0)
	{
		TextEffect = _Effect;
	}

	void SetSortType(EImageSortType _SortType)
	{
		SortType = _SortType;
	}

	void AutoImageScale(float _AutoScale = 1.0f)
	{
		AutoImageScaleValue = true;
		AutoImageScaleRatio = _AutoScale;
	}

protected:
	void BeginPlay() override;
	void Tick(float _Time) override;
private:
	int InfoIndex = 0;
	UWindowImage* Image = nullptr;
	bool ImageCuttingTransformSetting = false;
	FTransform ImageCuttingTransform;
	Color8Bit TransColor;

	bool CameraEffect = true;
	float CameraRatio = 1.0f;

	std::map<std::string, UAnimationInfo> AnimationInfos;
	UAnimationInfo* CurAnimation = nullptr;

	float Angle = 0.0f;

	std::string Text = "";
	std::string Font = "궁서";
	float Size = 10.0f;
	Color8Bit TextColor = Color8Bit::BlackA;
	Color8Bit TextColor2 = Color8Bit::BlackA;
	int TextEffect = 0;

	bool AutoImageScaleValue = false;
	float AutoImageScaleRatio = 1.0f;

	EImageSortType SortType = EImageSortType::Center;
};

