#pragma once

namespace ENG {
	struct EntityManager_t;
}

struct CameraSys_t {

	explicit CameraSys_t() = default;

	void update(ENG::EntityManager_t&) const;
};