#include "goFactory.hpp"
#include <app/cmp/inputCmp.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <app/cmp/renderCmp.hpp>
#include <app/cmp/collisionCmp.hpp>
#include <app/cmp/cameraCmp.hpp>
#include <app/cmp/AICmp.hpp>

ENG::Entity_t& GOFactory_t::createEntity(float x, float y) const
{
	auto& ent    = EntMan->createEntity();
	auto& phyCmp = EntMan->addCmp<PhysicsCmp_t>(ent);
	phyCmp.x = x;
	phyCmp.y = y;

	return ent;
}

ENG::Entity_t& GOFactory_t::createObserver(float x, float y) const
{
	auto& ent    = createEntity(x, y);
	auto& renCmp = EntMan->addCmp<RenderCmp_t>(ent);
	renCmp.loadFromPNG("./assets/observer.png");
	/*renCmp.width  = 8;
	renCmp.height = 8;
	renCmp.initFillSprite(0x00FFFFFF);*/

	auto& incmp = EntMan->addCmp<InputCmp_t>(ent);

	// Set key-action to player
    incmp.addAction(XK_Left, [&](PhysicsCmp_t& phycmp) {
    	phycmp.vAngular = -PhysicsCmp_t::MAX_VANGULAR;
    });
    incmp.addAction(XK_Right, [&](PhysicsCmp_t& phycmp) {
        phycmp.vAngular = PhysicsCmp_t::MAX_VANGULAR;
    });
    incmp.addAction(XK_Up, [&](PhysicsCmp_t& phycmp) {
    	phycmp.aLinear = PhysicsCmp_t::MAX_ALINEAR;
    });
    incmp.addAction(XK_Down, [&](PhysicsCmp_t& phycmp) {
        phycmp.aLinear = -PhysicsCmp_t::MAX_ALINEAR;
    });

	return ent;
}

ENG::Entity_t& GOFactory_t::createVisitor(float x, float y, ENG::EntID_t eid) const
{
	auto& ent = createEntity(x, y);

	auto& renCmp = EntMan->addCmp<RenderCmp_t>(ent);
	renCmp.loadFromPNG("./assets/visitor.png");
	/*renCmp.width  = 8;
	renCmp.height = 8;
	renCmp.initFillSprite(0x002874A6);*/

	//auto& collCmp = EntMan->addCmp<CollisionCmp_t>(ent);
	//collCmp.maskCollision = CollisionCmp_t::BOUNDARY_LAYER;
	//collCmp.boxRoot.box   = { 0, 0, renCmp.width-1, renCmp.height-1 };

	auto& AICmp     = EntMan->addCmp<AICmp_t>(ent);
	AICmp.eidTarget = eid;

	return ent;
}

ENG::Entity_t& 
GOFactory_t::createCamera(float x, float y, uint32_t w, uint32_t h, ENG::EntID_t eid) const
{
    auto& cam = EntMan->createEntity();

    auto& camcmp = EntMan->addCmp<CameraCmp_t>(cam);
    camcmp.xScr  = x; camcmp.yScr   = y;
    camcmp.width = w; camcmp.height = h;
    camcmp.followEntID = eid;

    [[maybe_unused]] auto& phycmp = EntMan->addCmp<PhysicsCmp_t>(cam);

    return cam;
}