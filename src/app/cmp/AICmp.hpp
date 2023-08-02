#pragma once
#include <engine/cmp/component.hpp>

struct AICmp_t : ENG::Component_t<AICmp_t>{

	enum class SB { ARRIVE, SEEK, FLEE };

	explicit AICmp_t(ENG::EntID_t eid)
	: Component_t(eid)
	{}

	ENG::EntID_t eidTarget;
	bool  targetActive { false };
	float radiusArrive {   1.0 };
	float timeArrive   {   1.0 };
	SB 	  stBehavior   { SB::ARRIVE };

	bool  destroy      { false };	// TO DEVS
};