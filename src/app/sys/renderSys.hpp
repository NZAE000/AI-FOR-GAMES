#pragma once
#include <cstdint>
#include <memory>
#include <app/util/box.hpp>
extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}

// forward declaration
namespace ENG {
	struct EntityManager_t;
}

struct PhysicsCmp_t;
struct RenderCmp_t;
struct CameraCmp_t;

struct RenderSys_t {

	struct CameraWithPhysics_t {
        const CameraCmp_t*  camcmp {nullptr};
        const PhysicsCmp_t* phycmp {nullptr};
        std::size_t eid;
    };

	static constexpr uint32_t RED           { 0xFFFF0000 };
    static constexpr uint32_t BACKGRD_COLOR { 0x0017202A };//  17202a

	explicit RenderSys_t(uint32_t w, uint32_t h) 
	: width_scr{w}, height_scr{h}, frameBuffer { std::make_unique<uint32_t[]>(width_scr*height_scr) }
	{
		ptc_open("Simulator Park", width_scr, height_scr);
	}

	~RenderSys_t() { ptc_close(); }

	void update(const ENG::EntityManager_t&) const;

private:
	void fillScreen(uint32_t color) const noexcept;
	void renderAllCameras(const ENG::EntityManager_t& entMan) const;
	void drawAllEntities(const ENG::EntityManager_t&) const ;
	void renderSpriteClipped(const RenderCmp_t&, const PhysicsCmp_t&) const noexcept;
	auto transformWorldCoordsToScreenRef(float x, float y, uint32_t width, uint32_t height) const noexcept;
	void drawSprite(uint32_t x, uint32_t y, uint32_t widthSpr, uint32_t heightSpr, uint32_t pixels_off, const uint32_t* ptrToSprite) const noexcept;
	void drawBox(const Box_t<uint32_t>&, uint32_t color) const noexcept;

	uint32_t* getPosition(uint32_t x, uint32_t y) const noexcept { 
		return frameBuffer.get() + this->width_scr*y + x; 
	}

	const uint32_t width_scr;
	const uint32_t height_scr;
	std::unique_ptr<uint32_t[]> frameBuffer;
	mutable CameraWithPhysics_t currentCam {};

};