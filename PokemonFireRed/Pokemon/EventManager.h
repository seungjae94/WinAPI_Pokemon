#pragma once
#include <map>
#include <vector>
#include <functional>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include <EngineCore/Level.h>
#include "PokemonMath.h"
#include "EventEnums.h"
#include "Pokemon.h"

class UPokemonLevel;
class AEventTarget;
class UEventTargetSetting;
class AEventTrigger;
class UEventProcessor;
class UEventStream;
class APlayerCharacter;
class UEventManagerReleaser;
class UEventCondition;
class ACanvas;
class ADialogueWindow;
class ATrainer;
enum class EFontColor;
enum class EPlayerState;

// 모든 이벤트 처리를 담당하는 클래스
// - 맵 레벨에서 액터를 조작하려면 반드시 EventManager 클래스를 통해서 조작해야 한다.
class UEventManager
{
	friend UEventProcessor;
	friend UPokemonLevel;
	friend UEventManagerReleaser;
public:
	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

	// 배틀 Enemy 데이터 세이브 로드
	static void SaveEnemyEntry(std::vector<UPokemon>* _Entry);
	static std::vector<UPokemon>* LoadEnemyEntry();
	static bool IsWildPokemonBattle();
	static void SetAsWildPokemonBattle();
	static void SetAsTrainerBattle(ATrainer* _Trainer);
	static ATrainer* GetTrainer()
	{
		return Trainer;
	}


	// 즉발 이벤트 함수
	// - 사이드 이펙트가 없는 이벤트 명령은 
	//   이벤트 트리거를 만들지 않고 즉시 실행해도 문제가 되지 않는다.

	static void ChangeArea(std::string_view _AreaName, std::string_view _AreaBgm);

	static void SetLevel(std::string_view _LevelName);

	static void SetPoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static void SetDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);
	
	static void SetActive(std::string_view _MapName, std::string_view _TargetName, bool _Value);

	static void SetCurLevelPlayerState(EPlayerState _State);

	static void FadeChangeLevel(std::string_view _TargetLevelName, bool _PlayerControl = false, float _FadeInTime = 0.3f, float _FadeOutTime = 0.3f, bool _IsFadeBgm = false, std::string_view _NewBgm = "");
	
	static void WildBattle(const FWildPokemonConstructorParam _ConstructorParam);

	static void OpenMenuWindow();

	static void OpenDialogueWindow(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14);

	static void ActivatePlayer();

	static void DeactivatePlayer();

	// 찾기 편의 함수
	static ADialogueWindow* FindCurLevelDialogueWindow();

	template <typename CanvasType>
	static CanvasType* FindCommonCanvas(std::string_view _LevelName, std::string_view _CanvasName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string CanvasName = UEngineString::ToUpper(_CanvasName);

		if (false == AllCommonCanvas.contains(LevelName) || false == AllCommonCanvas[LevelName].contains(CanvasName))
		{
			return nullptr;
		}

		ACanvas* Element = AllCommonCanvas[LevelName][CanvasName];
		CanvasType* Result = dynamic_cast<CanvasType*>(Element);
		
		if (nullptr == Result)
		{
			return nullptr;
		}

		return Result;
	}
	
	template <typename CanvasType>
	static CanvasType* FindCurLevelCommonCanvas(std::string_view _CanvasName)
	{
		return FindCommonCanvas<CanvasType>(CurLevelName, _CanvasName);
	}

	template <typename TargetType>
	static TargetType* FindTarget(std::string_view _LevelName, std::string_view _TargetName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string TargetName = UEngineString::ToUpper(_TargetName);

		if (false == AllTargets.contains(LevelName) || false == AllTargets[LevelName].contains(TargetName))
		{
			return nullptr;
		}

		AEventTarget* Target = AllTargets[LevelName][TargetName];
		TargetType* Result = dynamic_cast<TargetType*>(Target);

		if (nullptr == Result)
		{
			return nullptr;
		}

		return Result;
	}
	
	template <typename TargetType>
	static TargetType* FindCurLevelTarget(std::string_view _TargetName)
	{
		return FindTarget<TargetType>(CurLevelName, _TargetName);
	}

	// 주어진 위치의 모든 트리거를 반환
	template <typename TriggerType>
	static std::list<TriggerType*> FindTriggerListAt(std::string_view _LevelName, const FTileVector& _Point)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);

		if (false == AllTriggers.contains(LevelName) || false == AllTriggers[LevelName].contains(_Point))
		{
			return {};
		}

		std::list<AEventTrigger*> Triggers = AllTriggers[LevelName][_Point];
		std::list<TriggerType*> CastedTriggers;

		for (AEventTrigger* Trigger : Triggers)
		{
			TriggerType* Result = dynamic_cast<TriggerType*>(Trigger);

			if (nullptr != Result)
			{
				CastedTriggers.push_back(Result);
			}
		}
		
		return CastedTriggers;
	}

	template <typename TriggerType>
	static std::list<TriggerType*> FindCurLevelTriggerListAt(const FTileVector& _Point)
	{
		return FindTriggerListAt<TriggerType>(CurLevelName, _Point);
	}

	// 주어진 위치의 트리거를 하나만 찾아서 반환
	template <typename TriggerType>
	static TriggerType* FindTriggerAt(std::string_view _LevelName, const FTileVector& _Point)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);

		if (false == AllTriggers.contains(LevelName) || false == AllTriggers[LevelName].contains(_Point))
		{
			return {};
		}

		std::list<AEventTrigger*> Triggers = AllTriggers[LevelName][_Point];

		for (AEventTrigger* Trigger : Triggers)
		{
			TriggerType* Result = dynamic_cast<TriggerType*>(Trigger);

			if (nullptr != Result)
			{
				return Result;
			}
		}

		return nullptr;
	}

	template <typename TriggerType>
	static TriggerType* FindCurLevelTriggerAt(const FTileVector& _Point)
	{
		return FindTriggerAt<TriggerType>(CurLevelName, _Point);
	}

	static std::string GetCurLevelName()
	{
		return CurLevelName;
	}

	static float GetDeltaTime()
	{
		return DeltaTime;
	}

	// 제거 함수
	static void RemoveTarget(std::string_view _MapName, std::string_view _TargetName);
	static void RemoveTrigger(std::string_view _MapName, std::string_view _TargetName);

protected:
	// constructor destructor
	UEventManager();
	~UEventManager();
private:
	static std::string CurLevelName;
	static ULevel* BattleLevel;

	// AllMapLevelCanvas[LevelName][ElementName]
	// - 메뉴창도 커서 이동 등 플레이어 타입으로 다뤄야 할 일이 있기 때문에 추가로 보관한다.
	static std::map<std::string, std::map<std::string, ACanvas*>> AllCommonCanvas;

	static std::map<std::string, ADialogueWindow*> AllDialogueWindows;

	// AllTargets[LevelName][TargetName]
	// - 'A레벨의 B라는 이름의 액터를 이동시켜줘'라는 요청을 처리하려면 A레벨의 B라는 이름의 액터를 찾을 수 있어야 한다.
	static std::map<std::string, std::map<std::string, AEventTarget*>> AllTargets;

	// AllTriggers[LevelName][Point] = list of Triggers
	// - 플레이어가 특정 위치에 트리거가 있는지 확인할 때 사용
	// - 해당 트리거를 동작시킬 때 사용
	static std::map<std::string, std::map<FTileVector, std::list<AEventTrigger*>>> AllTriggers;
	
	// 배틀 정보 저장
	static bool IsWildPokemon;
	static std::vector<UPokemon>* EnemyEntry;
	static ATrainer* Trainer;

	// 틱
	static void Tick(float _DeltaTime);
	
	// 등록 함수
	static void AddTarget(AEventTarget* _Target, const UEventTargetSetting& _Setting);
	static void AddTrigger(AEventTrigger* _Trigger, const UEventTargetSetting& _Setting);
	static void AddPlayer(APlayerCharacter* _Player, const FTileVector& _Point);
	static void AddCommonCanvas(ACanvas* _Canvas, std::string_view _Name);
	static void AddDialogueWindow(ADialogueWindow* _Window);

	// DeltaTime 기록
	static float DeltaTime;
};