#include "renderSys.hpp"
#include <algorithm>
#include <engine/man/entityMan.hpp>
#include <app/cmp/renderCmp.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <app/cmp/cameraCmp.hpp>
#include <iostream>
#include <tuple>
#include <optional>
#include <cmath>


auto RenderSys_t::
transformWorldCoordsToScreenRef(float x, float y, uint32_t width, uint32_t height) const noexcept
{
    // CURRENT IMPLEMETATION (camera added)
    // COORDINATES TRANSFORMATION:

    //   SPRT     -->     WORLD      -->      CAMERA      -->     SCREEN
    //      (0,0 + POSSPR)       (-CAMPOSWRLD)         (+CAMPOSSCR)

    std::tuple<uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t> tupla {0,0,0,0,0,0};
    auto optTuple { std::optional{tupla} };

    // VERIFY AND GET CURRENT CAMERA COMPONENTS
    if (!currentCam.camcmp || !currentCam.phycmp) { optTuple.reset(); return optTuple; }
    auto& camCmp      = *currentCam.camcmp;
    auto& phyCmpOfCam = *currentCam.phycmp;

    struct SprRefTo {   // Sprite coordinates reference ..

        Box_t<float> world  {}; // to world,
        Box_t<float> camera {}; // to camera,
        
        struct CroppedOnCam {      // pixels to crop if the sprite is not completely relative to the camera
            uint32_t left_off { 0 }, right_off { 0 };
            uint32_t up_off   { 0 }, down_off  { 0 };
        } croppedOnCam {};

        struct Screen {            // and coordinates ref to screen.
            uint32_t x {0}, y {0};
        } screen{};

    } sprRef {};

    // TRANSFORM SPRITE COORDINATES INTO 'WORLD' REF (0,0 + POSSPRITE)
    sprRef.world = { // {xSup, ySup, xInf, yInf}
            x
        ,   y
        ,   x + width  - 1
        ,   y + height - 1
    };

    // TRANSFORM SPRITE COORDINATES INTO 'CAMERA' REF (POSSPRITEWRLD - POSCAMWRLD)
    sprRef.camera = {
            sprRef.world.getXLeft()  - phyCmpOfCam.x
        ,   sprRef.world.getYUp()    - phyCmpOfCam.y
        ,   sprRef.world.getXRight() - phyCmpOfCam.x
        ,   sprRef.world.getYDown()  - phyCmpOfCam.y
    };

    // Get xleft, xright, yup and ydown coordinates ref on camera
    float sprCamXLeft  { sprRef.camera.getXLeft()  };
    float sprCamXRight { sprRef.camera.getXRight() };
    float sprCamYUp    { sprRef.camera.getYUp()    };
    float sprCamYDown  { sprRef.camera.getYDown()  };

    // Check the coordinates of the sprite referring to the camera is out of bounds of camera.
    if (   sprCamXLeft > (camCmp.width -1) || sprCamXRight < 0
        || sprCamYUp   > (camCmp.height-1) || sprCamYDown  < 0 )
    {
        optTuple.reset(); return optTuple;
    }

    // Maybe only part of the sprite is in the camera..
    sprRef.croppedOnCam = {
            static_cast<uint32_t>(std::round((sprCamXLeft  < 0               ) ?  -sprCamXLeft                  : 0)) // p.ejem: si left esta a -3px en x de la camara, se debe recortar 3 pixeles (-(-3)=3).))
        ,   static_cast<uint32_t>(std::round((sprCamXRight > (camCmp.width-1)) ?  sprCamXRight-(camCmp.width-1) : 0))
        ,   static_cast<uint32_t>(std::round((sprCamYUp    < 0               ) ?  -sprCamYUp                    : 0))
        ,   static_cast<uint32_t>(std::round((sprCamYDown  > (camCmp.height-1))?  sprCamYDown-(camCmp.height-1) : 0))
    };

    // TRANSFORM SPRITE COORDINATES INTO SCREEN REF (sprite refcam coord + cam refscreen coord) (clipped) AND NEW DIMENSIONS
    sprRef.screen = {
            camCmp.xScr + static_cast<uint32_t>(std::round(sprCamXLeft) + sprRef.croppedOnCam.left_off)  // x
        ,   camCmp.yScr + static_cast<uint32_t>(std::round(sprCamYUp)   + sprRef.croppedOnCam.up_off)    // y
    };

    /*if (currentCam.eid == 1) { // only show player coords
    std::cout<<"\ncamXscr: "<<camCmp.xScr<<" camYscr: "<<camCmp.yScr<<"\n";
    std::cout<<"camXwrld: " <<phyCmpOfCam.x<<" camYwrld: "<<phyCmpOfCam.y<<"\n";
    std::cout<<"xrefWrld: " <<x<<" yrefWrld: "<<y<<"\n";
    std::cout<<"xrefCam: "  <<sprRef.camera.xSup<<" yrefCam: "<<sprRef.camera.ySup<<"\n";
    std::cout<<"xrefScr: "  <<sprRef.screen.x<<" yrefScr: "<<sprRef.screen.y<<"\n";
    }*/

    // CTAD: class template argument deduction:: since C++17.
    std::tuple t {
            sprRef.screen.x,               sprRef.screen.y
        ,   sprRef.croppedOnCam.left_off,  sprRef.croppedOnCam.right_off
        ,   sprRef.croppedOnCam.up_off,    sprRef.croppedOnCam.down_off
    };
    (*optTuple).swap(t);
    
    return optTuple;
}


void RenderSys_t::update(const ENG::EntityManager_t& entMan) const
{
	fillScreen(BACKGRD_COLOR);
	renderAllCameras(entMan);
	ptc_update(frameBuffer.get());
}

void RenderSys_t::fillScreen(uint32_t color) const noexcept
{
    auto* buff = frameBuffer.get();
    std::fill(buff, buff + (width_scr*height_scr), color);
}

void RenderSys_t::
renderAllCameras(const ENG::EntityManager_t& entMan) const
{
    auto& camcmps = entMan.template getCmps<CameraCmp_t>();
    for (const auto& camcmp : camcmps)
    {
        auto* phycmp = entMan.template getRequiredCmp<PhysicsCmp_t>(camcmp);
        if (!phycmp) continue;
    	
        // SET CUURENT CAM
        currentCam.camcmp = &camcmp;
        currentCam.phycmp = phycmp;

        drawAllEntities(entMan);  // ON CAMERA!!
        //renderCamFrame();          // RENDER BOUNDS OF CAM
    }
}

void RenderSys_t::drawAllEntities(const ENG::EntityManager_t& entMan) const
{
	const auto& renCmps = entMan.getCmps<RenderCmp_t>();
	//auto getPositionXY = [&](uint32_t x, uint32_t y){ return frameBuffer.get() + y*width_scr + x; };

	for (const auto& rencmp : renCmps)
	{
		const auto* phycmp = entMan.template getRequiredCmp<PhysicsCmp_t>(rencmp);
		if (phycmp)
		{
			currentCam.eid = phycmp->getEntityID();
            renderSpriteClipped(rencmp, *phycmp); // ON CAMERA!!

			/*auto* ptr_toScr = getPosition(phycmp->x, phycmp->y);
			auto ptr_toSpr = rencmp.sprite.begin();
			for (uint32_t y=0; y<rencmp.height; ++y){
				std::copy(ptr_toSpr, ptr_toSpr + rencmp.width, ptr_toScr);
				ptr_toSpr += rencmp.width;
				ptr_toScr += width_scr;
			}*/
		}

	}
}

void RenderSys_t::
renderSpriteClipped(const RenderCmp_t& rencmp, const PhysicsCmp_t& phycmp) const noexcept
{   
    auto optTuple { transformWorldCoordsToScreenRef(phycmp.x, phycmp.y, rencmp.width, rencmp.height) };
    if (!optTuple) return;
    
    // New coords sprite to 'screen reference' and pixels_off for sprite redimension.
    auto [xScr, yScr, l_off, r_off, u_off, d_off] = *optTuple;

    // Pixels to crop
    uint32_t sidePixelsOff { l_off + r_off };
    uint32_t suprPixelsOff { u_off + d_off };

    // New sprite dimensions (smaller or equal)
    uint32_t newWidth  { rencmp.width - sidePixelsOff  };
    uint32_t newHeight { rencmp.height - suprPixelsOff };

    // RENDER SPRITE
    const uint32_t* sprite_it { rencmp.sprite.data() + u_off*rencmp.width + l_off };

    drawSprite(xScr, yScr, newWidth, newHeight, sidePixelsOff, sprite_it);

    if (!phycmp.immovable && !(l_off || r_off || u_off || d_off) ){
        uint32_t bw   { 5 };
        uint32_t xSup { static_cast<uint32_t>(xScr + (newWidth/2.0)  * (1.0 + std::cos(phycmp.orientation)) - 1.0 - (bw-1)/2.0)};
        uint32_t ySup { static_cast<uint32_t>(yScr + (newHeight/2.0) * (1.0 + std::sin(phycmp.orientation)) - 1.0 - (bw-1)/2.0)};
        uint32_t xInf { xSup + (bw - 1) };
        uint32_t yInf { ySup + (bw - 1) };
        Box_t<uint32_t> boxDirection { xSup, ySup, xInf, yInf};
        drawBox(boxDirection, RED);
    }
}

void RenderSys_t::
drawSprite(uint32_t x, uint32_t y, uint32_t widthSpr, uint32_t heightSpr, uint32_t pixels_off, const uint32_t* ptrToSprite) const noexcept
{
	auto* ptrToBuff = getPosition(x, y);

    while(heightSpr--) {
        for (uint32_t w=0; w<widthSpr; ++w)
        {
            // Draw only if transparency != 0 (not blending)
            if (*ptrToSprite & 0xFF000000)
            *ptrToBuff = *ptrToSprite;
            ++ptrToSprite; ++ptrToBuff;
        }
        ptrToSprite += pixels_off;
        ptrToBuff   += this->width_scr - widthSpr;
    }
}

void RenderSys_t:: 
drawBox(const Box_t<uint32_t>& box, uint32_t color) const noexcept
{
    auto* ptrToBuff = getPosition(box.xSup, box.ySup);
    uint32_t W { box.getWidth()  };
    uint32_t H { box.getHeight() };

    while (H--)
    {
        for (uint32_t w=0; w<W; ++w){
            *ptrToBuff = color;
            ++ptrToBuff;
        }
        ptrToBuff += this->width_scr - W;
    }    
}