#pragma once
#include <engine/cmp/component.hpp>


struct CameraCmp_t : ENG::Component_t<CameraCmp_t> {

	explicit CameraCmp_t(ENG::EntID_t eid)
	: Component_t(eid) 
	{}

	uint32_t xScr {0}, yScr {0};
	uint32_t width {0}, height {0};

	ENG::EntID_t followEntID;

};