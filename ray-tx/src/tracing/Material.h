/*
    program name: ray-compute, computes rays for multiple lights and materials
    Copyright (C) 2022  a-cpu-a

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    you can contact me electronicly by making a issue at https://github.com/a-cpu-a/ray-compute/issues/new
*/
#pragma once


#include <cstdint>
#include <vector>

#include "../WaveLenght.h"

struct SpectrumSegment
{
	WaveLenght min = 0;
	WaveLenght max = 0;

	// = { {0,UINT16_MAX} };
	constexpr SpectrumSegment() {}
	constexpr SpectrumSegment(const uint16_t& _min, const uint16_t& _max) : min(_min), max(_max) {}

};



typedef uint8_t Refration;//MAX = 4.984375 //MIN = 1 //DIV 64


struct MaterialData
{
	std::vector<SpectrumSegment> absorbtionData;//for coloring the material

	std::vector<SpectrumSegment> transparencyData;//for glass
	std::vector<Refration> transparencySegmentInfo;//refraction strength data?

	uint8_t roughness = 0xFF;//the less the smoother & more mirror like
};
