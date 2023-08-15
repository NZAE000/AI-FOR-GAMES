#pragma once
#include <engine/cmp/component.hpp>
#include <app/util/point2d.hpp>

struct AICmp_t : ENG::Component_t<AICmp_t>{

	enum class SB { ARRIVE, SEEK, FLEE, PURSUE };

	explicit AICmp_t(ENG::EntID_t eid)
	: Component_t(eid)
	{}

	Point2D_t pointTarget { 0, 0};
	bool  targetActive  { false };
	float arrivalRadius {   1.0 };
	float arrivalTime   {   1.0 };
	SB 	  stBehavior    { SB::ARRIVE };
	
	ENG::EntID_t eidTarget;
};