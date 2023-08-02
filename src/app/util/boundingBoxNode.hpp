#pragma once
#include <cstdint>
#include <vector>
#include <app/util/box.hpp>


// Composite pattern
struct BoundingBNode_t {
		
	Box_t<uint32_t> box;
	bool isCollided { false };
	std::vector<BoundingBNode_t> subBoxes;
};