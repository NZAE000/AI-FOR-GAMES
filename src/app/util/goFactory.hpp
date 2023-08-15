#pragma once

#include <engine/man/entityMan.hpp>
#include <string_view>
#include <cstdint>

struct GOFactory_t {

	explicit GOFactory_t() = default;

	ENG::Entity_t& createEntity(float x, float y)   const;
	ENG::Entity_t& createObserver(float x, float y) const;
	ENG::Entity_t& createAI(float x, float y)       const;
	ENG::Entity_t& createCamera(float x, float y, uint32_t w, uint32_t h, ENG::EntID_t eid) const;


	void setMan(ENG::EntityManager_t& entMan) noexcept { EntMan = &entMan; }
	ENG::EntityManager_t& getMan() noexcept { return *EntMan; }

private:
	ENG::EntityManager_t* EntMan { nullptr };
};