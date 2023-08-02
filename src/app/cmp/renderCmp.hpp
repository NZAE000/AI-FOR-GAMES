#pragma once

#include <engine/cmp/component.hpp>
#include <engine/util/typeAliasess.hpp>
#include <string_view>
#include <cstdint>
#include <algorithm>

struct RenderCmp_t : ENG::Component_t<RenderCmp_t>{

	explicit RenderCmp_t(ENG::EntID_t eid)
	: Component_t{eid} {}
	

	void initFillSprite(uint32_t color) noexcept
	{
		sprite.resize(width*height);
		std::fill(sprite.begin(), sprite.end(), color);
	}


	void initSprite(uint32_t w, uint32_t h, const uint32_t* spr) noexcept
	{
		sprite.resize(w*h);
		auto ptr_toSpr = sprite.begin();
		std::copy(spr, spr + w*h, ptr_toSpr);
		width = w; height = h;
	}

	void loadFromPNG(std::string_view pathFile);
	auto loadPNGintoVector(std::string_view pathFile);
	void initSpriteFromABGRData(const std::vector<unsigned char>& pixels);

	uint32_t width {0}, height{0};
	std::vector<uint32_t> sprite;
};
