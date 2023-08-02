#pragma once
#include <engine/cmp/component.hpp>
#include <engine/util/typeAliasess.hpp>
#include <functional>
#include <X11/X.h>
#include <X11/keysym.h>

struct PhysicsCmp_t; // forward declaration

struct InputCmp_t : ENG::Component_t<InputCmp_t> {

	explicit InputCmp_t (ENG::EntID_t eid)
	: Component_t(eid) 
	{}

	template<typename CALLABLE>
	constexpr void addAction(KeySym key, CALLABLE action) // operator [] cant throw excpt.
	{
		actions[key] = action;
	}

	constexpr const auto& getActions() const noexcept { return actions; }

	// Mapping to key - action
	ENG::Hash_t<KeySym, std::function<void(PhysicsCmp_t&)>> actions;
};
