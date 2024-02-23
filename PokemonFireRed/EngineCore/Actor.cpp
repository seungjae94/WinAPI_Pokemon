#include "Actor.h"
#include "ImageRenderer.h"

AActor::AActor()
{
}

AActor::~AActor()
{
	for (UImageRenderer* Renderer : Renderers)
	{
		if (nullptr == Renderer)
		{
			MsgBoxAssert("이미지 렌더러가 nullptr인 상황이 존재합니다.");
		}

		delete Renderer;
		Renderer = nullptr;
	}
	Renderers.clear();

	for (UCollision* Collision : Collisions)
	{
		if (nullptr == Collision)
		{
			MsgBoxAssert("콜리전이 nullptr인 상황이 존재합니다.");
		}

		delete Collision;
		Collision = nullptr;
	}
	Collisions.clear();
}

UImageRenderer* AActor::CreateImageRenderer(int _Order)
{
	UImageRenderer* Component = new UImageRenderer();
	UActorComponent* ActorCom = Component;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(_Order);
	ActorCom->BeginPlay();
	Renderers.push_back(Component);
	return Component;
}

UCollision* AActor::CreateCollision(int _Order)
{
	UCollision* Component = new UCollision();
	UActorComponent* ActorCom = Component;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(_Order);
	ActorCom->BeginPlay();
	Collisions.push_back(Component);
	return Component;
}

void AActor::Destroy(float _DestroyTime)
{
	UTickObject::Destroy(_DestroyTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->Destroy(_DestroyTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->Destroy(_DestroyTime);
	}
}

void AActor::DestroyUpdate(float _DeltaTime)
{
	UTickObject::DestroyUpdate(_DeltaTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->DestroyUpdate(_DeltaTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->DestroyUpdate(_DeltaTime);
	}
}

void AActor::SetActive(bool _Active, float _ActiveTime)
{
	UTickObject::SetActive(_Active, _ActiveTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->SetActive(_Active, _ActiveTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->SetActive(_Active, _ActiveTime);
	}
}

void AActor::ActiveUpdate(float _DeltaTime)
{
	UTickObject::ActiveUpdate(_DeltaTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveUpdate(_DeltaTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->ActiveUpdate(_DeltaTime);
	}
}

void AActor::CheckReleaseChild()
{
	{
		std::list<UImageRenderer*>::iterator StartIter = Renderers.begin();
		std::list<UImageRenderer*>::iterator EndIter = Renderers.end();

		for (; StartIter != EndIter;)
		{
			UImageRenderer* Renderer = StartIter.operator*();

			if (nullptr == Renderer)
			{
				MsgBoxAssert("Renderer가 nullptr인 경우가 존재했습니다");
				return;
			}

			if (false == Renderer->IsDestroy())
			{
				++StartIter;
				continue;
			}

			delete Renderer;
			Renderer = nullptr;
			StartIter = Renderers.erase(StartIter);
		}
	}

	{
		std::list<UCollision*>::iterator StartIter = Collisions.begin();
		std::list<UCollision*>::iterator EndIter = Collisions.end();

		for (; StartIter != EndIter;)
		{
			UCollision* Collision = StartIter.operator*();

			if (nullptr == Collision)
			{
				MsgBoxAssert("Collision이 nullptr인 경우가 존재했습니다");
				return;
			}

			if (false == Collision->IsDestroy())
			{
				++StartIter;
				continue;
			}

			delete Collision;
			Collision = nullptr;
			StartIter = Collisions.erase(StartIter);
		}
	}
}

void AActor::AllRenderersActiveOff()
{
	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveOff();
	}
}

void AActor::AllRenderersActiveOn()
{
	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveOn();
	}
}

void AActor::Tick(float _DeltaTime)
{
	UTickObject::Tick(_DeltaTime);
	DestroyUpdate(_DeltaTime);
}
