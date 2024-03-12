#pragma once
#include "Animator.h"

class APlayerTackleAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		PlayerMoveRight,
		ShowTackleEffect,
		PlayerMoveLeft,
		HideTackleEffect,
		WaitBlinkEffectEnd,
		End,
	};
public:
	// constructor destructor
	APlayerTackleAnimator();
	~APlayerTackleAnimator();

	// delete Function
	APlayerTackleAnimator(const APlayerTackleAnimator& _Other) = delete;
	APlayerTackleAnimator(APlayerTackleAnimator&& _Other) noexcept = delete;
	APlayerTackleAnimator& operator=(const APlayerTackleAnimator& _Other) = delete;
	APlayerTackleAnimator& operator=(APlayerTackleAnimator&& _Other) noexcept = delete;

	void Start() override;
	bool IsEnd() override;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	AImageElement* TackleEffect = nullptr;
	
	// ����
	EState State = EState::None;
	void ProcessPlayerMoveRight();
	void ProcessShowTackleEffect();
	void ProcessPlayerMoveLeft();
	void ProcessHideTackleEffect();
	void ProcessWaitBlinkEffectEnd();

	// ������
	FVector PlayerRightPos;
	const float MoveTime = 0.1f;
	const float BeforeBlinkEffectTime = 0.25f;
	float Timer = 0.0f;
};
