#include "physicSys.hpp"
#include <engine/man/entityMan.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <cmath>

void PhysicSys_t::update(ENG::EntityManager_t& entMan, float dt) const
{
	auto& phyCmps = entMan.getCmps<PhysicsCmp_t>();
	for (auto& phycmp : phyCmps)
	{
		float newOrien { phycmp.orientation + phycmp.vAngular*dt };

		if      (newOrien > 2*PhysicsCmp_t::PI)	 phycmp.orientation = newOrien - 2*PhysicsCmp_t::PI;
		else if (newOrien < 0)  				 phycmp.orientation = newOrien + 2*PhysicsCmp_t::PI;
		else									 phycmp.orientation = newOrien;

		// cos = catAd/hipo => catAd = hipo*cos
		phycmp.vx = phycmp.vLinear*std::cos(phycmp.orientation);
		phycmp.vy = phycmp.vLinear*std::sin(phycmp.orientation);

		// x = v*t
		phycmp.x += phycmp.vx*dt;
		phycmp.y += phycmp.vy*dt;

		// v = a*t 		//(acelerar solo en velocidad lineal, en velocidad angular mucho descontrol)
		phycmp.vLinear  += phycmp.aLinear*dt;
		phycmp.vLinear   = std::clamp(phycmp.vLinear,  -PhysicsCmp_t::MAX_VLINEAR,  PhysicsCmp_t::MAX_VLINEAR);
		//phycmp.vAngular += phycmp.aAngular*dt;
		//phycmp.vAngular  = std::clamp(phycmp.vAngular, -PhysicsCmp_t::MAX_VANGULAR, PhysicsCmp_t::MAX_VANGULAR);
	
		// Drag 
		double drag { phycmp.vLinear*phycmp.friction };
		phycmp.vLinear -= drag*dt;
	}
}