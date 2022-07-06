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

#include <glm/glm.hpp>

#include "Scene.h"
#include "tracing/Material.h"
#include "Random64.h"

std::array<MaterialData, 256> getMaterials()
{
	std::array<MaterialData, 256> ret;
	for (MaterialData& data : ret)
	{
		data = MaterialData();
		data.absorbtionData = {};
		data.transparencyData = {};
		data.transparencySegmentInfo = {};
		data.roughness = 0;
	}

	ret[0].roughness = 0;//air
	ret[0].absorbtionData = { 
		{0,16384},{24030,31348},{31894,32768},
		{33805,34843},{35170,24030},
		{37191,38229},{38939,40031},{42598,43690},
		{44182,45656},{48168,51882},
		{54613,55159},{57343,61166}
	};
	ret[0].transparencyData = { SpectrumSegment(0, UINT16_MAX) };
	ret[0].transparencySegmentInfo = { 0 };

	ret[1].roughness = 10;//gold
	ret[1].absorbtionData = { {0,10923},{12506,13926},{16329,18677},{19224,21299},{23210,26105} };
	//MULTIPLY THESES ^^^ BY 54.6125, CUZ FIXED POINT

	ret[2].roughness = 0xFF;//snow
	ret[2].absorbtionData = { {3877,4314},{4915,7973},{8738,22337},{41615,43690},{54613,56251},{58326,59364},{62258,64934} };

	ret[3].roughness = 0;//water
	ret[3].absorbtionData = { {3877,4314},{4915,7973},{8738,22337},{41615,43690},{54613,56251},{58326,59364},{62258,64934} };
	ret[3].transparencyData = { SpectrumSegment(0, UINT16_MAX) };
	ret[3].transparencySegmentInfo = { 21 };//1.33333-

	ret[4].roughness = 100;//white
	ret[4].absorbtionData = { };

	return ret;
}

static const std::array<MaterialData, 256> matrialList = getMaterials();

struct LightPoint
{
	uint32_t rayCount = 1;

	uint16_t temperature = 4600;//light bulb   |  5788;//sun
	glm::u16vec3 pos = {0,0,0};

	Material material = 0;
	bool useMaterial = false;
};
static const constexpr std::array<LightPoint, 1> LIGHT_POINTS = { LightPoint(20000,5788,{60,180,60}) };

inline uint16_t blackBody(const WaveLenght& waveLenght,const uint16_t& temperature)
{
	if (waveLenght > 1092)//20*54.6125
		return UINT16_MAX;
	else
		return 3;

}

inline WaveLenght getLightWaveForLight(const LightPoint& data, uint64_t& seed)
{
	if (data.useMaterial)
	{
		return 1092;//20*54.6125
	}
	else
	{
		//black body radition
		for (size_t i = 0; i < 1000; i++)
		{
			const WaveLenght waw = randU16(seed);
			if (blackBody(waw, data.temperature) > randU16(seed))
				return waw;
		}
		return 1093;// 20*54.6125+1
	}
}

consteval size_t countRaysFromLights()
{
	size_t ret = 0;
	for (const LightPoint& lightData : LIGHT_POINTS)
	{
		ret += lightData.rayCount;
	}
	return ret;
}
consteval uint32_t countMaxRaysFromLights()
{
	uint32_t ret = 0;
	for (const LightPoint& lightData : LIGHT_POINTS)
	{
		ret = glm::max(lightData.rayCount,ret);
	}
	return ret;
}

static const constinit size_t THREAD_COUNT = 4;

static const constinit size_t TOTAL_RAY_COUNT = countRaysFromLights();
static const constinit uint32_t MAX_LIGHT_RAY_COUNT = countMaxRaysFromLights();
static const constinit uint32_t MAX_LIGHT_RAY_COUNT_PER_THREAD = MAX_LIGHT_RAY_COUNT / THREAD_COUNT + ((MAX_LIGHT_RAY_COUNT % THREAD_COUNT)!=0);
//static const constinit size_t TOTAL_RAY_COUNT_PER_THREAD = TOTAL_RAY_COUNT / THREAD_COUNT + ((TOTAL_RAY_COUNT % THREAD_COUNT) != 0);

inline Scene loadedScene;
