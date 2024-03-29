#include "EnemyTackleAnimator.h"
#include "BlinkEffectAnimator.h"
#include "ShakeEffectAnimator.h"

AEnemyTackleAnimator::AEnemyTackleAnimator()
{
}

AEnemyTackleAnimator::~AEnemyTackleAnimator()
{
}

void AEnemyTackleAnimator::Start()
{
	AAnimator::Start();

	State = EState::EnemyMoveLeft;
	Timer = MoveTime;

	// �ʱ�ȭ
	TackleEffect->SetActive(false);
	EnemyLeftPos = EnemyInitPos + UPokemonUtil::PixelVector(-10, 0);
}

bool AEnemyTackleAnimator::IsEnd()
{
	return State == EState::End;
}

void AEnemyTackleAnimator::SetTypeVs(ETypeVs _TypeVs)
{
	TypeVs = _TypeVs;
}

void AEnemyTackleAnimator::BeginPlay()
{
	AAnimator::BeginPlay();

	TackleEffect = CreateImageElement(this, ERenderingOrder::UI6, EPivotType::LeftBot, 55, -50);
	TackleEffect->SetImage(RN::TackleEffect);
}

void AEnemyTackleAnimator::Tick(float _DeltaTime)
{
	AAnimator::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::None:
		break;
	case EState::EnemyMoveLeft:
		ProcessEnemyMoveLeft();
		break;
	case EState::ShowTackleEffect:
		ProcessShowTackleEffect();
		break;
	case EState::EnemyMoveRight:
		ProcessEnemyMoveRight();
		break;
	case EState::HideTackleEffect:
		ProcessHideTackleEffect();
		break;
	case EState::WaitBlinkEffectEnd:
		ProcessWaitBlinkEffectEnd();
		break;
	case EState::End:
		break;
	default:
		break;
	}
}

void AEnemyTackleAnimator::ProcessEnemyMoveLeft()
{
	FVector Pos = UPokemonMath::Lerp(EnemyLeftPos, EnemyInitPos, Timer / MoveTime);
	GetEnemy()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::ShowTackleEffect;
		USoundManager::PlaySE(RN::SETackle);
	}
}

void AEnemyTackleAnimator::ProcessShowTackleEffect()
{
	State = EState::EnemyMoveRight;
	Timer = MoveTime;
	TackleEffect->SetActive(true);
	ShakeEffectAnimator->Start(true);
}

void AEnemyTackleAnimator::ProcessEnemyMoveRight()
{
	FVector Pos = UPokemonMath::Lerp(EnemyInitPos, EnemyLeftPos, Timer / MoveTime);
	GetEnemy()->SetRelativePosition(Pos);

	if (Timer <= 0.0f)
	{
		State = EState::HideTackleEffect;
		TackleEffect->SetActive(false);
	}
}

void AEnemyTackleAnimator::ProcessHideTackleEffect()
{
	if (true == ShakeEffectAnimator->IsEnd())
	{
		State = EState::WaitBlinkEffectEnd;
		BlinkEffectAnimator->Start(true, 0.5f);

		Timer = DamageSoundWaitTime;
		IsDamageSoundPlayed = false;
	}
}

void AEnemyTackleAnimator::ProcessWaitBlinkEffectEnd()
{
	if (false == IsDamageSoundPlayed && Timer <= 0.0f)
	{
		switch (TypeVs)
		{
		case ETypeVs::NotVeryEffective:
			USoundManager::PlaySE(RN::SENotVeryEffectiveDamage);
			break;
		case ETypeVs::NormallyEffective:
			USoundManager::PlaySE(RN::SENormalDamage);
			break;
		case ETypeVs::SuperEffective:
			USoundManager::PlaySE(RN::SESuperEffectiveDamage);
			break;
		default:
			break;
		}

		IsDamageSoundPlayed = true;
	}

	if (true == BlinkEffectAnimator->IsEnd())
	{
		State = EState::End;
	}
}
