#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EngineCore/Actor.h>

class UImageRenderer;

class ATitleLevelManager : public AActor
{
public:
	// constructor destructor
	ATitleLevelManager();
	~ATitleLevelManager();

	// delete Function
	ATitleLevelManager(const ATitleLevelManager& _Other) = delete;
	ATitleLevelManager(ATitleLevelManager&& _Other) noexcept = delete;
	ATitleLevelManager& operator=(const ATitleLevelManager& _Other) = delete;
	ATitleLevelManager& operator=(ATitleLevelManager&& _Other) noexcept = delete;

protected:
private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UImageRenderer* Renderer = nullptr;

	int VideoIndex = 0;
	const std::string VideoName[5] = { "1st", "2nd", "3rd", "4th", "5th"};

	void Video0Logic();
	void Video1Logic();
	void Video2Logic();
	void Video3Logic(float _DeltaTime);
	void Video4Logic();

	void PlayNextVideo();

	bool IsAnyKeyboardDown();
};
