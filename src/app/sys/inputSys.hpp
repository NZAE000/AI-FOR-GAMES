#pragma once

#include <engine/util/keyBoard.hpp>

namespace ENG {
	struct EntityManager_t;
}

struct InputSys_t {

	explicit InputSys_t();

	void update(ENG::EntityManager_t&) const;
	bool isKeyPress(KeySym) const noexcept;
	bool isExitEvent() const noexcept;
	void setOn() const noexcept;

private:
	inline static ENG::KeyBoard_t keyboard {};
	
	static void onKeyPress(KeySym)   noexcept;
	static void onKeyRelease(KeySym) noexcept;

};