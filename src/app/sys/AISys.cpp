#include "AISys.hpp"
#include <engine/man/entityMan.hpp>
#include <app/cmp/AICmp.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <tuple>
#include <cmath>

constexpr auto AISys_t::
getDistanceToTarget(Point2D_t const& pOrigin, Point2D_t const& pTarget) const
{
	float disxT     { pTarget.x - pOrigin.x };
	float disyT     { pTarget.y - pOrigin.y };
	float distanceT { std::sqrt(disxT*disxT + disyT*disyT) };

	return std::tuple(disxT, disyT, distanceT);
}

constexpr double AISys_t::
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

constexpr AISys_t::SteerTarget_t AISys_t::
alignAngle(float originOrien, float targetOrien, float timeArrive) const
{
	SteerTarget_t steerT {};

	// ANGULAR VELOCITY
	float vAngToTarget  { targetOrien - originOrien };

	// SEARCH BEST RUTE
	adjustBestAngle(vAngToTarget);

	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float vAngSpecificToTarget { vAngToTarget / timeArrive };
	steerT.angular = std::clamp(vAngSpecificToTarget, -PhysicsCmp_t::MAX_VANGULAR, PhysicsCmp_t::MAX_VANGULAR);

	// ANGULAR ACCELERATION
	//float aAngTspecific { (vAngSpecificToTarget - phycmp->vAngular) / aicmp.timeArrive };
	//phycmp->aAngular = std::clamp(aAngTspecific, -PhysicsCmp_t::MAX_AANGULAR, PhysicsCmp_t::MAX_AANGULAR);

	return steerT;
}

void AISys_t::
arrive(AICmp_t& aicmp, ENG::EntityManager_t& entMan) const
{
	if (!aicmp.targetActive) return;
	auto* phycmp    = entMan.getRequiredCmp<PhysicsCmp_t>(aicmp);
	if (!phycmp)    return;
	auto* entTarget = entMan.getEntityByID(aicmp.eidTarget);
	if (!entTarget) return;
	auto* phycmpT   = entTarget->getComponent<PhysicsCmp_t>();
	if (!phycmpT)   return;

	phycmp->aLinear = phycmp->vAngular = 0;

// DISTANCE TARGET
	auto [disxT, disyT, distanceT] = getDistanceToTarget({phycmp->x, phycmp->y}, {phycmpT->x, phycmpT->y});
	
	if (distanceT <= aicmp.radiusArrive) {		// Check if AI is on target
		aicmp.targetActive = false;
		if (aicmp.destroy) entMan.destroyEntity(aicmp.getEntityID()); // Check if it is necessary to destroy
		return;
	}

// TARGET ORIENTATION
	float orienTarget  { calculateAngle({disxT, disyT}) };
	auto steerToTarget { alignAngle(phycmp->orientation, orienTarget, aicmp.timeArrive) };
	phycmp->vAngular = steerToTarget.angular;

// LINEAR VELOCITY
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float vLinTSpecific { distanceT / aicmp.timeArrive };
	vLinTSpecific = std::clamp(vLinTSpecific, -PhysicsCmp_t::MAX_VLINEAR, PhysicsCmp_t::MAX_VLINEAR);

// LINEAR ACCELERATION
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float aLinTSpecific { (vLinTSpecific - phycmp->vLinear) / aicmp.timeArrive  };
	phycmp->aLinear = std::clamp(aLinTSpecific, -PhysicsCmp_t::MAX_ALINEAR, PhysicsCmp_t::MAX_ALINEAR);
}

void AISys_t::
seek(PhysicsCmp_t& phycmp, AISys_t::Point2D_t const& pointT, float timeArrive) const
{
// DISTANCE TO TARGET
	auto [disxT, disyT, distanceT] = getDistanceToTarget({phycmp.x, phycmp.y}, {pointT.x, pointT.y});

// ORIENTATION TO TARGET
	float orienTarget  { calculateAngle({disxT, disyT}) };
	auto steerToTarget { alignAngle(phycmp.orientation, orienTarget, timeArrive) };
	phycmp.vAngular = steerToTarget.angular;

// LINEAR ACCELERATION
	float aLinear { PhysicsCmp_t::MAX_VLINEAR / (1 + std::fabs(steerToTarget.angular)) };
	// Divide per specific time to arrive (if it is not divided, it is taken as a reference in a second)
	float aLinTSpecific { aLinear / timeArrive };
	phycmp.aLinear = std::clamp(aLinTSpecific, -PhysicsCmp_t::MAX_ALINEAR, PhysicsCmp_t::MAX_ALINEAR);
}

// It's SEEK but it should move away
void AISys_t::
flee(PhysicsCmp_t& phycmp, Point2D_t const& pTargetToFlee, float timeArrive) const
{
	// DISTANCE TO TARGET (should move away)
	AISys_t::Point2D_t pTarget { phycmp.x + (phycmp.x - pTargetToFlee.x), phycmp.y + (phycmp.y - pTargetToFlee.y) };
	seek(phycmp, pTarget, timeArrive);
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

		switch(aicmp.stBehavior)
		{
		case AICmp_t::SB::ARRIVE:	arrive(aicmp, entMan);
			break;
		case AICmp_t::SB::SEEK:		seek(*phycmp, {phycmpT->x, phycmpT->y}, aicmp.timeArrive);
			break;
		case AICmp_t::SB::FLEE:		flee(*phycmp, {phycmpT->x, phycmpT->y}, aicmp.timeArrive);
			break;
		default:
			break;
		}
	}
}
