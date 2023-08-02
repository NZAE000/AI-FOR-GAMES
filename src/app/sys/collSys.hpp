#pragma once

#include <app/util/boundingBoxNode.hpp>

namespace ENG {
	struct EntityManager_t;
}

struct PhysicsCmp_t;

struct CollisionSys_t {

	explicit CollisionSys_t(uint32_t w_scr, uint32_t h_scr)
	: wScreen{w_scr}, hScreen{h_scr}
	{}

	void update(ENG::EntityManager_t&) const;

private:
	const uint32_t wScreen, hScreen;

	constexpr Box_t<float>
	transformToWorldCoordinates(const Box_t<uint32_t>& box, float x, float y) const noexcept;
	constexpr void checkBoundingScreenCollision(Box_t<uint32_t>const&, PhysicsCmp_t&) const noexcept;
};