#pragma once
#include "Warp.h"

class AInteriorDoor : public AWarp
{
public:
	// constructor destructor
	AInteriorDoor();
	~AInteriorDoor();

	// delete Function
	AInteriorDoor(const AInteriorDoor& _Other) = delete;
	AInteriorDoor(AInteriorDoor&& _Other) noexcept = delete;
	AInteriorDoor& operator=(const AInteriorDoor& _Other) = delete;
	AInteriorDoor& operator=(AInteriorDoor&& _Other) noexcept = delete;

	void SetExteriorDoorName(std::string_view _Name)
	{
		ExteriorDoorName = _Name;
	}
	void RegisterPredefinedEvent() override;

protected:

private:
	std::string ExteriorDoorName;
};

