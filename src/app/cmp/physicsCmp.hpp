#pragma once

#include <engine/cmp/component.hpp>
#include <engine/util/typeAliasess.hpp>
#include <numbers>

struct PhysicsCmp_t : ENG::Component_t<PhysicsCmp_t> {

	static constexpr auto  PI      { std::numbers::pi_v<float> };
	static constexpr float MAX_VLINEAR   { 				   100 }; // MAX Linear velocity to 100 PX per sec
	static constexpr float MAX_VANGULAR  {               PI*.5 };
	static constexpr float MAX_ALINEAR   { 		MAX_VLINEAR/.5 };
	static constexpr float MAX_AANGULAR  { 	   MAX_VANGULAR/.5 };

	explicit PhysicsCmp_t(ENG::EntID_t eid)
	: Component_t {eid}
	{}

	float x{0}, y{0};
	float vx{0}, vy{0};
	float orientation{0};

	float vLinear{0}, vAngular{0};
	float aLinear{0}, aAngular{0};

	float friction{.95};

	uint8_t immovable {0}; // If a entity is static
};