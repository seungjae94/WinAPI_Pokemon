#pragma once
#include <map>
#include <list>
#include <EngineBase/NameObject.h>
#include <EngineCore/EngineCore.h>

// ���� ������ ������ ���� ������ ���͸� ���� �����ϸ� ��ȯ ���� ������ �߻��� �� �ִ�.
// Ŭ���� ���� �������� ��ȯ ���� ������ ȸ���Ѵ�.
class AActor;

// ���͵��� Ȱ���� ����
class ULevel : public UNameObject
{
	friend EngineCore;
public:
	// constructor destructor
	ULevel();

	// ������ ���͸� �����ϹǷ� ������ ���͸� �������ؾ� �Ѵ�.
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	virtual void BeginPlay() 
	{
	}

	virtual void Tick(float _DeltaTime) 
	{
	}

	// ������ ���͸� �������ִ� �Լ�
	// - ������ ���带 ���� ������ �����ϰ�, ������ BeginPlay�� ȣ���Ѵ�.
	// - ���͸� map�� �����Ѵ�.
	template<typename ActorType>
	ActorType* SpawnActor(int _Order = 0)
	{
		ActorType* NewActor = new ActorType();
		ActorInit(NewActor);
		AllActor[_Order].push_back(NewActor);
		return NewActor;
	}

protected:

private:
	std::map<int, std::list<AActor*>> AllActor;

	// ������ ���带 ���� ������ �����ϰ�, ������ BeginPlay�� ȣ���Ѵ�.
	void ActorInit(AActor* _Actor);

	// ������Ʈ(����) ƽ ȣ��
	void LevelTick(float _DeltaTime);

	// ������: ���� ����
	void LevelRender(float _DeltaTime);

	// Destroy�� ������Ʈ(����) ������
	void LevelRelease(float _DeltaTime);
};
