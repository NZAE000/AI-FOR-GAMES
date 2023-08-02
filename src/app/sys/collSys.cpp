#include "collSys.hpp"
#include <engine/man/entityMan.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <app/cmp/collisionCmp.hpp>

constexpr Box_t<float> CollisionSys_t::
transformToWorldCoordinates(const Box_t<uint32_t>& box, float x, float y) const noexcept
{   
    return Box_t<float> {
            x + box.getXLeft()
        ,   y + box.getYUp()
        ,   x + box.getXRight()
        ,   y + box.getYDown()
    };
}

constexpr void CollisionSys_t::checkBoundingScreenCollision(const Box_t<uint32_t>& box, PhysicsCmp_t& phycmp) const noexcept
{
	// Bounding coordinates convertion to screen coordinates
    Box_t<float> boxTransToSrc = transformToWorldCoordinates(box, phycmp.x, phycmp.y);
    
    float xL { boxTransToSrc.getXLeft()  };
    float xR { boxTransToSrc.getXRight() };
    float yU { boxTransToSrc.getYUp()    };
    float yD { boxTransToSrc.getYDown()  };

    // Horizontal boundig verification
    if (xL < 0 || xR >= wScreen) {
        //phycmp.vx *= -1; 
    }
    // Vertical boundig verification
    if (yU < 0  || yD >= hScreen) {
    	//phycmp.vy *= -1;
    }
}

void CollisionSys_t::update(ENG::EntityManager_t& entMan) const
{

	auto& collCmps = entMan.getCmps<CollisionCmp_t>();

	for (auto& collcmp : collCmps)
	{
		auto* phycmp = entMan.template getRequiredCmp<PhysicsCmp_t>(collcmp);
		if (!phycmp) continue;

		if (collcmp.maskCollision & CollisionCmp_t::BOUNDARY_LAYER)
			checkBoundingScreenCollision(collcmp.boxRoot.box, *phycmp);
	}
}