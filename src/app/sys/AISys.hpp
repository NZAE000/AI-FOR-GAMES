#pragma once

namespace ENG {
	struct EntityManager_t;
}

struct AICmp_t;
struct PhysicsCmp_t;

struct AISys_t {

	explicit AISys_t() = default;
	void update(ENG::EntityManager_t&) const;

private:

	struct Point2D_t { float x{0.0}, y{0.0}; };	

	// MRU (only velocity control) or MRUA (acceleration control) on linear and angular
	struct SteerTarget_t {
		float linear  {0.0};
		float angular {0.0};
	};

	// Steering Behavior
	void arrive(AICmp_t& aicmp, ENG::EntityManager_t&) const;
	void seek(PhysicsCmp_t&, Point2D_t const&, float timeArrive) const;
	void flee(PhysicsCmp_t&, Point2D_t const&, float timeArrive) const;

	constexpr auto getDistanceToTarget(Point2D_t const&, Point2D_t const&) const;
	constexpr float calculateAngle(Point2D_t const&) const;
	constexpr SteerTarget_t alignAngle(float originOrien, float targetOrien, float timeArrive) const;
	constexpr double adjustBestAngle(float& angle) const;
};