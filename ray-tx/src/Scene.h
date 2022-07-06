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
#include <array>
#include <vector>


/*struct Material
{
	uint8_t id = 0;

	constexpr Material() {}
	constexpr Material(uint8_t _id) : id(_id) {}
};*/
typedef uint8_t Material;

#define HORIZONTAL 256
#define VERTICAL 192
//!! MAXIMUM SIZE IS UINT16_MAX

struct Scene
{
	std::array<std::array<std::array<Material, HORIZONTAL>, VERTICAL>, HORIZONTAL> voxels;
};
