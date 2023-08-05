#pragma once
#include <engine/cmp/component.hpp>

struct AICmp_t : ENG::Component_t<AICmp_t>{

	enum class SB { ARRIVE, SEEK, FLEE, PURSUE };

	explicit AICmp_t(ENG::EntID_t eid)
	: Component_t(eid)
	{}

	ENG::EntID_t eidTarget;
	bool  targetActive  { false };
	float arrivalRadius {   1.0 };
	float arrivalTime   {   1.0 };
	SB 	  stBehavior    { SB::ARRIVE };

	bool  destroy      { false };	// TO DEVS
};