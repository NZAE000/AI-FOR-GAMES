#include "renderCmp.hpp"
#include <fstream>
#include <tuple>
#include <picoPNG.ua/src/picopng.hpp>
extern "C" {
	#include<tinyPTC.ua/src/tinyptc.h>
}

auto RenderCmp_t::loadPNGintoVector(std::string_view pathFile)
{	

	std::ifstream inFile(pathFile.data(), std::ios::binary);
	if (!inFile) throw std::runtime_error("file can't open");
      
	std::vector<unsigned char> fileVec (
		std::istreambuf_iterator<char>(inFile),
		std::istreambuf_iterator<char>{}
	);
	
	unsigned long width{0}, height{0};
	std::vector<unsigned char> pixels;

	decodePNG(pixels, width, height, fileVec.data(), fileVec.size());
	
	// CTAD: Class template argument deduction. Sice c++17
	return std::tuple {static_cast<uint32_t>(width),static_cast<uint32_t>(height), pixels};	
}

void RenderCmp_t::initSpriteFromABGRData(const std::vector<unsigned char>& pixels)
{
        sprite.reserve(pixels.size()/4);

    // Ahora se debe almacenar pixels uchar en mi vector uint32_t sprite de entity.
    // Coger cada uno de los 4 bytes y desplazarlos a su posicion dentro de los 32 bits de un uint32_t
    // para que esten en el formato correcto.
    for (auto px = pixels.begin(); px != pixels.end(); px+=4)
    {
        uint32_t pixel =
            static_cast<uint32_t>(*(px+0)) << 16
        |   static_cast<uint32_t>(*(px+1)) <<  8
        |   static_cast<uint32_t>(*(px+2))
        |   static_cast<uint32_t>(*(px+3)) << 24;
        sprite.emplace_back(pixel);
    }
    //std::memcpy(sprite.data(), pixels.data(), pixels.size());
    //sprite = std::vector<uint32_t>(pixels.begin(), pixels.end());
}

void RenderCmp_t::loadFromPNG(std::string_view pathFile)
{
        auto [mw, mh, pixels] = loadPNGintoVector(pathFile);
        initSpriteFromABGRData(pixels);

        width = mw; height = mh;
}
