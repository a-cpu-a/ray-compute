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
#include <string>
#include <vector>
#include <fstream>
#include <ios>

#include "Ray.h"
#include "Color.h"

/*

struct ComputedRay
{
	glm::vec3 pos = {0.0f,0.0f,0.0f};
	glm::vec3 dir = { 0.0f,0.0f,0.0f };

	uint16_t waveLenght = 0;//0 to 1200
};*/

inline void save(const std::string& path, const std::vector<Ray>* rays)
{
	std::ofstream file(path, std::ios::binary);

	uint64_t rayCount = (uint64_t)rays->size();
	file.write((char*)&rayCount, sizeof(uint64_t));

	for (const Ray& ray: *rays)
	{
		float a = ray.pos.x;
		float b = ray.pos.y;
		float c = ray.pos.z;

		int16_t d = ray.dir.x;
		int16_t e = ray.dir.y;
		int16_t f = ray.dir.z;

		uint16_t g = ray.waveLenght;

		file.write((char*) & a, sizeof(float));
		file.write((char*) & b, sizeof(float));
		file.write((char*) & c, sizeof(float));

		file.write((char*) & d, sizeof(int16_t));
		file.write((char*) & e, sizeof(int16_t));
		file.write((char*) & f, sizeof(int16_t));

		file.write((char*)&g, sizeof(uint16_t));

	}


	file.close();
}
