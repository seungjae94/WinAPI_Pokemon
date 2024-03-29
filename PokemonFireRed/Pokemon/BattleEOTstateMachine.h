#pragma once
#include "BattleStateMachine.h"
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ABattleEOTStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		TestTempStatus,
		TempStatusAnim,
		LeechSeed,
		Bind,
		TempStatusMessage1,
		TempStatusMessage2,
		TestStatus,
		StatusMessage1,
		StatusMessage2,
		StatusAnim,
		StatusDamage,
		End
	};

	enum class ELeechSeedState
	{
		None,
		Damage,
		Heal,
	};
public:
	// constructor destructor
	ABattleEOTStateMachine();
	~ABattleEOTStateMachine();

	// delete Function
	ABattleEOTStateMachine(const ABattleEOTStateMachine& _Other) = delete;
	ABattleEOTStateMachine(ABattleEOTStateMachine&& _Other) noexcept = delete;
	ABattleEOTStateMachine& operator=(const ABattleEOTStateMachine& _Other) = delete;
	ABattleEOTStateMachine& operator=(ABattleEOTStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(UBattler* _Target, UBattler* _CounterTarget);

protected:
	void Tick(float _DeltaTime) override;
private:
	void Start() override {};
	UBattler* Target = nullptr;
	UBattler* CounterTarget = nullptr;
	ESubstate State = ESubstate::None;

	// 고유 데이터
	float Timer = 0.0f;
	const float TempStatusAnimTime = 1.0f;
	const float StatusAnimTime = 1.0f;
	const float HpBarDecreaseTime = 1.0f;
	const float BattleMsgShowTime = 0.75f;
	ELeechSeedState LeechSeedState = ELeechSeedState::None;

	// 상태 틱 함수
	void ProcessTestTempStatus();
	void ProcessTempStatusAnim();
	void ProcessLeechSeed();
	void ProcessBind();
	void ProcessTempStatusMessage1();
	void ProcessTempStatusMessage2();
	void ProcessTestStatus();
	void ProcessStatusMessage1();
	void ProcessStatusMessage2();
	void ProcessStatusAnim();
	void ProcessStatusDamage();
};

