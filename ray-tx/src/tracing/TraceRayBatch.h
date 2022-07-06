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

#include "TraceRay.h"
#include "../Ray.h"
#include "../Globals.h"

inline void traceRayBatch(const std::array<Ray, MAX_LIGHT_RAY_COUNT_PER_THREAD> rays,const uint32_t count, std::vector<Ray>* ret)
{
	//std::array<Ray, 1000> ret;

	uint16_t seed = count;

	for (size_t i = 0; i < count; i++)
	{
		const TracedRay out = traceRay(rays[i], seed);

		if (out.save)
			ret->push_back(out.data);

	}
}
