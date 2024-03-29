#pragma once
#include "SceneComponent.h"
#include <vector>

class UCollision : public USceneComponent
{
public:
	// constrcuter destructer
	UCollision();
	~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	void SetColType(ECollisionType _Type)
	{
		ColType = _Type;
	}

	void SetOrder(int _Order) override;

	// 충돌 순서가 _Order인 충돌체와 충돌하는지 여부를 반환한다.
	// 충돌할 경우 충돌체 전부를 _Result에 담아준다. 
	template<typename EnumType>
	bool CollisionCheck(EnumType _Order, std::vector<UCollision*>& _Result, FVector _NextPos = FVector::Zero)
	{
		return CollisionCheck(static_cast<int>(_Order), _Result, _NextPos);
	}

	// 충돌 순서가 _Order인 충돌체와 충돌하는지 여부를 반환한다.
	// 충돌할 경우 충돌체 전부를 _Result에 담아준다. 
	bool CollisionCheck(int _Order, std::vector<UCollision*>& _Result, FVector _NextPos = FVector::Zero);

	void DebugRender(FVector _CameraPos);

protected:
	void BeginPlay() override;

private:
	ECollisionType ColType = ECollisionType::Rect;
};

