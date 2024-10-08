#include "EventTrigger.h"

#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"
#include "EventManager.h"
#include "EventProcessor.h"
#include "EventStream.h"

AEventTrigger::AEventTrigger() 
{
}

AEventTrigger::~AEventTrigger() 
{
    UnregisterEvents();
}

void AEventTrigger::RegisterEvent(EEventTriggerAction _Action, const std::function<bool()>& _Cond, const UEventStream& _Stream)
{
    if (false == AllStreams.contains(_Action))
    {
        AllStreams.emplace(_Action, std::list<FCondAndStream>());
    }

    AllStreams[_Action].push_back(FCondAndStream( _Cond, &_Stream));
}

bool AEventTrigger::TriggerEvent(EEventTriggerAction _Action)
{
    if (true == UEventProcessor::IsRunning())
    {
        return false;
    }

    for (FCondAndStream& CondAndStream : AllStreams[_Action])
    {
        if (true == CondAndStream.Cond())
        {
            UEventProcessor::Run(this, CondAndStream.Stream);
            return true;
        }
    }

    return false;
}

void AEventTrigger::UnregisterEvents()
{
    for (std::pair<const EEventTriggerAction, std::list<FCondAndStream>>& Streams : AllStreams)
    {
        Streams.second.clear();
    }

    AllStreams.clear();
}
