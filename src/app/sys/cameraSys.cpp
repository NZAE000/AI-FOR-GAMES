#include "cameraSys.hpp"
#include <engine/man/entityMan.hpp>
#include <app/cmp/cameraCmp.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <app/cmp/renderCmp.hpp>

void CameraSys_t::update(ENG::EntityManager_t& entMan) const
{
	auto& camcmps = entMan.template getCmps<CameraCmp_t>();

	for(auto& camcmp : camcmps)
	{
		auto* phycmpCam = entMan.template getRequiredCmp<PhysicsCmp_t>(camcmp);
		if (!phycmpCam) continue;

		auto* followEntity = entMan.getEntityByID(camcmp.followEntID);
		if (!followEntity) continue;

		auto* phycmpEnt = followEntity->template getComponent<PhysicsCmp_t>();
		if (!phycmpEnt) continue;

		auto* rencmpEnt = followEntity->template getComponent<RenderCmp_t>();
		if (!rencmpEnt) continue;

		phycmpCam->x = phycmpEnt->x - (camcmp.width  - rencmpEnt->width)/2; 
		phycmpCam->y = phycmpEnt->y - (camcmp.height - rencmpEnt->height)/2;
	}
}