#pragma once
#include <engine/cmp/component.hpp>
#include <engine/util/typeAliasess.hpp>
#include <app/util/boundingBoxNode.hpp>

struct CollisionCmp_t : ENG::Component_t<CollisionCmp_t> {

	// Para que solo entre algunas entidades halla colision
	enum { NO_LAYER = 0X00, BOUNDARY_LAYER = 0X01, FULL_LAYER = 0xFF };

	explicit CollisionCmp_t (ENG::EntID_t eid)
	: Component_t(eid) 
	{}

	BoundingBNode_t boxRoot;
	uint8_t maskCollision { NO_LAYER }; // 0 = 0000 0000 : COLISIONA CON NADA POR DEFAULT
	//uint8_t property { NO_PROP };
};
