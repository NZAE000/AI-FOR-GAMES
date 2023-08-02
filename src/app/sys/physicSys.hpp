#pragma once


namespace ENG {
	struct EntityManager_t;
}

struct PhysicSys_t {

	explicit PhysicSys_t() = default;

	void update(ENG::EntityManager_t&, float dt) const;

};