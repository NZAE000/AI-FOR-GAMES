#include "AISys.hpp"
#include <engine/man/entityMan.hpp>
#include <app/cmp/AICmp.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <tuple>
#include <cmath>

constexpr auto AISys_t::
calculatePointDistance(Point2D_t const& pOrigin, Point2D_t const& pTarget) const
{
	float disxT     { pTarget.x - pOrigin.x };
	float disyT     { pTarget.y - pOrigin.y };
	float distanceT { std::sqrt(disxT*disxT + disyT*disyT) };

	return std::tuple(disxT, disyT, distanceT);
}

constexpr float AISys_t::
adjustBestAngle(float& angle) const
{
	while (angle > PhysicsCmp_t::PI)  angle -= 2*PhysicsCmp_t::PI;
	while (angle < -PhysicsCmp_t::PI) angle += 2*PhysicsCmp_t::PI;
	return angle;
}

constexpr float AISys_t::
calculateAngle(AISys_t::Point2D_t const& point) const
{
	float orien { std::atan2(point.y, point.x) };
	if (orien < 0) orien += 2*PhysicsCmp_t::PI; // not negative radian
	return orien;
}

constexpr float AISys_t::
alignAngle(float originOrien, float targetOrien, float arrivalTime) const
{
	// ANGULAR VELOCITY
	float vAngToTarget  { targetOrien - originOrien };

	// SEARCH BEST RUTE
	adjustBestAngle(vAngToTarget);

	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float vAngSpecificToTarget { vAngToTarget / arrivalTime };
	vAngSpecificToTarget = std::clamp(vAngSpecificToTarget, -PhysicsCmp_t::MAX_VANGULAR, PhysicsCmp_t::MAX_VANGULAR);

	// ANGULAR ACCELERATION
	//float aAngTspecific { (vAngSpecificToTarget - phycmp->vAngular) / aicmp.arrivalTime };
	//aAngTspecific = std::clamp(aAngTspecific, -PhysicsCmp_t::MAX_AANGULAR, PhysicsCmp_t::MAX_AANGULAR);

	return vAngSpecificToTarget; // ONLY KINEMATIC ON THIS MOMENT!!
}

AISys_t::SteerTarget_t AISys_t::
arrive(PhysicsCmp_t const& phycmp, AISys_t::Point2D_t const& pointT, float arrivalTime, float arrivalRadius) const
{
// DISTANCE TARGET
	auto [disxT, disyT, distanceT] = calculatePointDistance({phycmp.x, phycmp.y}, {pointT.x, pointT.y});
	
	if (distanceT <= arrivalRadius) {		// Check if AI is on target
		//aicmp.targetActive = false;
		//if (aicmp.destroy) entMan.destroyEntity(aicmp.getEntityID()); // Check if it is necessary to destroy
		return {};
	}

// ANGULAR VELOCITY
	float orienTarget  { calculateAngle({disxT, disyT}) };
	float angularSteer { alignAngle(phycmp.orientation, orienTarget, arrivalTime) };

// LINEAR VELOCITY
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float vLinTSpecific { distanceT / arrivalTime };
	vLinTSpecific = std::clamp(vLinTSpecific, -PhysicsCmp_t::MAX_VLINEAR, PhysicsCmp_t::MAX_VLINEAR);

// LINEAR ACCELERATION
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float aLinTSpecific { (vLinTSpecific - phycmp.vLinear) / arrivalTime  };
	float linearSteer   { std::clamp(aLinTSpecific, -PhysicsCmp_t::MAX_ALINEAR, PhysicsCmp_t::MAX_ALINEAR) };

	return { linearSteer, angularSteer };
}

AISys_t::SteerTarget_t AISys_t::
seek(PhysicsCmp_t const& phycmp, AISys_t::Point2D_t const& pointT, float arrivalTime) const
{
// DISTANCE TO TARGET
	auto [disxT, disyT, distanceT] = calculatePointDistance({phycmp.x, phycmp.y}, {pointT.x, pointT.y});

// ANGULAR VELOCITY
	float orienTarget  { calculateAngle({disxT, disyT}) };
	float angularSteer { alignAngle(phycmp.orientation, orienTarget, arrivalTime) };

// LINEAR ACCELERATION
	float aLinear { PhysicsCmp_t::MAX_VLINEAR / (1 + std::fabs(angularSteer)) }; // The more aligned the angle is to your target, the acceleration increases
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float aLinTSpecific { aLinear / arrivalTime };
	float linearSteer { std::clamp(aLinTSpecific, -PhysicsCmp_t::MAX_ALINEAR, PhysicsCmp_t::MAX_ALINEAR) };

	return { linearSteer, angularSteer };
}

// It's SEEK but it should move away
AISys_t::SteerTarget_t AISys_t::
flee(PhysicsCmp_t const& phycmp, Point2D_t const& pTargetToFlee, float arrivalTime) const
{
	// DISTANCE TO TARGET (should move away)
	const AISys_t::Point2D_t pTarget { phycmp.x + (phycmp.x - pTargetToFlee.x), phycmp.y + (phycmp.y - pTargetToFlee.y) };
	return seek(phycmp, pTarget, arrivalTime);
}

void AISys_t::update(ENG::EntityManager_t& entMan) const
{
	auto& aiCmps = entMan.getCmps<AICmp_t>();
	for(auto& aicmp : aiCmps)
	{
		if (!aicmp.targetActive) return;
		auto* phycmp    = entMan.getRequiredCmp<PhysicsCmp_t>(aicmp);
		if (!phycmp)    return;
		auto* entTarget = entMan.getEntityByID(aicmp.eidTarget);
		if (!entTarget) return;
		auto* phycmpT   = entTarget->getComponent<PhysicsCmp_t>();
		if (!phycmpT)   return;

		phycmp->aLinear = phycmp->vAngular = 0;
		SteerTarget_t steer;

		switch(aicmp.stBehavior)
		{
		case AICmp_t::SB::ARRIVE:	steer = arrive(*phycmp, {phycmpT->x, phycmpT->y}, aicmp.arrivalTime, aicmp.arrivalRadius);
			break;
		case AICmp_t::SB::SEEK:		steer = seek(*phycmp, {phycmpT->x, phycmpT->y}, aicmp.arrivalTime);
			break;
		case AICmp_t::SB::FLEE:		steer = flee(*phycmp, {phycmpT->x, phycmpT->y}, aicmp.arrivalTime);
			break;
		default:
			break;
		}

		phycmp->aLinear  = steer.linear;
		phycmp->vAngular = steer.angular;
	}
}
