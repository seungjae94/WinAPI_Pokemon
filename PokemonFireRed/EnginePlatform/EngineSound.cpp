#include "EngineSound.h"
#include <EngineBase\EngineString.h>
#include <EngineBase\EngineDebug.h>

// Debug
#pragma comment(lib, "fmodL_vc.lib")

// Release
// #pragma comment(lib, "fmod_vc.lib")

std::map<std::string, UEngineSound*> UEngineSound::Resources;
float UEngineSound::GlobalVolume = 1.0f;

void UEngineSoundPlayer::SetVolume(float _Volume)
{
	Control->setVolume(_Volume * UEngineSound::GlobalVolume);
}

// 사운드 로드, 정보 획득 등 사운드 관리에 사용하는 핸들(권한)
FMOD::System* SoundSystem = nullptr;

class ResControl
{
public:
	ResControl()
	{
		if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
		{
			MsgBoxAssert("사운드 시스템 생성에 실패했습니다.");
			return;
		}

		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgBoxAssert("사운드 시스템 생성에 실패했습니다.");
			return;
		}
	}
	~ResControl()
	{
		UEngineSound::ResourcesRelease();
		SoundSystem->release();
		SoundSystem = nullptr;
	}
};

ResControl Inst;

void UEngineSound::ResourcesRelease()
{
	for (std::pair<const std::string, UEngineSound*>& Pair : Resources)
	{
		delete Pair.second;
	}

	Resources.clear();
}

UEngineSound::UEngineSound()
{
}

UEngineSound::~UEngineSound()
{
}


void UEngineSound::ResLoad(std::string_view _Path)
{
	SoundSystem->createSound(_Path.data(), FMOD_LOOP_NORMAL, nullptr, &SoundHandle);
	if (nullptr == SoundHandle)
	{
		MsgBoxAssert("사운드 로드에 실패했습니다." + std::string(_Path));
		return;
	}
}

void UEngineSound::SetGlobalVolume(float _Value)
{
	GlobalVolume = _Value;

	if (GlobalVolume <= 0.0f)
	{
		GlobalVolume = 0.0f;
	}

	if (GlobalVolume >= 1.0f)
	{
		GlobalVolume = 1.0f;
	}
}

UEngineSoundPlayer UEngineSound::SoundPlay(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == Resources.contains(UpperName))
	{
		MsgBoxAssert("로드하지 않은 사운드를 재생하려고 했습니다." + UpperName);
		return UEngineSoundPlayer();
	}

	UEngineSound* FindSound = Resources[UpperName];

	UEngineSoundPlayer Result;
	SoundSystem->playSound(FindSound->SoundHandle, nullptr, false, &Result.Control);
	Result.Control->setLoopCount(0);
	Result.SetVolume(1.0f);

	if (nullptr == Result.Control)
	{
		MsgBoxAssert("사운드 재생에 대한 제어 권한을 가져오지 못했습니다.");
		return Result;
	}


	return Result;
}

void UEngineSound::Update()
{
	if (nullptr == SoundSystem)
	{
		MsgBoxAssert("사운드 시스템에 치명적인 오류가 있습니다.");
	}

	// 매프레임마다 호출되어야 한다.
	// FMOD 라이브러리가 여러 사운드를 알아서 제어해준다.
	SoundSystem->update();
}

void UEngineSound::Load(std::string_view _Path, std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == Resources.contains(UpperName))
	{
		MsgBoxAssert("이미 로드한 리소스를 또 로드 하려고 했습니다.");
		return;
	}

	UEngineSound* NewSound = new UEngineSound();
	NewSound->ResLoad(_Path);
	Resources[UpperName] = NewSound;
}