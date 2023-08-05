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
	SteerTarget_t arrive(PhysicsCmp_t const&, AISys_t::Point2D_t const&, float arrivalTime, float arrivalRadius) const;
	SteerTarget_t seek(PhysicsCmp_t const&, Point2D_t const&, float arrivalTime) const;
	SteerTarget_t flee(PhysicsCmp_t const&, Point2D_t const&, float arrivalTime) const;
	SteerTarget_t pursue(PhysicsCmp_t const&, PhysicsCmp_t const&, float arrivalTime) const;

	constexpr auto calculatePointDistance(Point2D_t const&, Point2D_t const&) const;
	constexpr float calculateAngle(Point2D_t const&) const;
	constexpr float alignAngle(float originOrien, float targetOrien, float arrivalTime) const;
	constexpr float adjustBestAngle(float& angle) const;
};