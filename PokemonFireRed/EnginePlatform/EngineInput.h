#pragma once

#include <Windows.h>
#include <map>
#include <EngineBase/EngineDebug.h>

class UEngineInput
{
	friend class InputInitCreator;
private:
	class EngineKey
	{
		friend UEngineInput;
	public:
		bool Down = false;	// 이번 프레임에 키를 눌렀는가?
		bool Up = false;	// 이번 프레임에 키를 뗐는가?
		bool Press = false; // 키가 눌려있는가?
		bool Free = true;   // 키가 떼어져있는가?

		float PressTime = 0.0f;
		float UpTime = 0.0f;

		int Key = -1;		// 키의 이름

		// GetAsyncKeyState 함수의 반환값을 이용해 키의 상태를 갱신해주는 함수
		void KeyCheck(float _DeltaTime);

		EngineKey()
		{
		}

		EngineKey(int _Key)
			: Key(_Key)
		{
		}
	};
public:
	// constructor destructor
	UEngineInput();
	~UEngineInput();

	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;

	// 키 상태 확인 함수
	static bool IsDoubleClick(int _Key, float _ClickTime)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력설정이 존재하지 않는 키 입니다");
		}

		bool Value = AllKeys[_Key].Down;
		float Time = AllKeys[_Key].UpTime;

		if (true == AllKeys[_Key].Down && AllKeys[_Key].UpTime < _ClickTime)
		{
			return true;
		}

		return false;
	}

	static bool IsDown(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Down;
	}

	static float GetPressTime(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다.");
		}

		return AllKeys[_Key].PressTime;
	}

	static bool IsPress(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Press;
	}

	static bool IsUp(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Up;
	}

	static bool IsFree(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Free;
	}

	// '아무 키나 누르세요' 기능을 위한 함수
	static bool IsAnykeyDown()
	{
		return AnykeyDown;
	}
	static bool IsAnykeyPress()
	{
		return AnykeyPress;
	}
	static bool IsAnykeyUp()
	{
		return AnykeyUp;
	}
	static bool IsAnykeyFree()
	{
		return AnykeyFree;
	}

	// 매 프레임마다 EngineCore::EngineTick에서 호출해주는 함수
	static void KeyCheckTick(float _DeltaTime);

protected:
	static std::map<int, EngineKey> AllKeys;

	static bool AnykeyDown;
	static bool AnykeyPress;
	static bool AnykeyUp;
	static bool AnykeyFree;

private:
	// AllKeys에 키 객체를 매핑해주는 함수
	static void InputInit();
};

