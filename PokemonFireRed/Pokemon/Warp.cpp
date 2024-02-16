#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>
#include "PokemonDebug.h"
#include "PokemonMath.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "Player.h"

AWarp::AWarp() 
{
}

AWarp::~AWarp() 
{
}

bool AWarp::CheckPlayerDirection()
{
	FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
	return CurPlayerDirection == TargetDirection;
}
