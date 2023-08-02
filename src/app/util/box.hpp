#pragma once
#include <cstdint>

template<typename Type>
struct Box_t {


	constexpr Type getXLeft()  const noexcept { return xSup; }
	constexpr Type getXRight() const noexcept { return xInf; }

	constexpr Type getYUp()    const noexcept { return ySup; }
	constexpr Type getYDown()  const noexcept { return yInf; }

	constexpr Type getWidth()  const noexcept { return (xInf - xSup) + 1; }
	constexpr Type getHeight() const noexcept { return (yInf - ySup) + 1; }

	Type xSup {}, ySup {};
	Type xInf {}, yInf {};
};