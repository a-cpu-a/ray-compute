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
#include <glm/glm.hpp>

#include "WaveLenght.h"

struct Ray
{
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };//6
	glm::i16vec3 dir = { 0.0f, 0.0f, 0.0f };//6

	WaveLenght waveLenght = 0;//MAX = 1200  //2

	//uint16_t sus = 0;

	//s = 14! (min)
};
