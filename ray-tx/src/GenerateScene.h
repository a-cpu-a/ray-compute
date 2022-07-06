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

#include <FastNoiseLite.h>

#include "Globals.h"
#include "Scene.h"

inline void generateScene()
{
	/*for (size_t i = 0; i < (HORIZONTAL * HORIZONTAL); i++)
	{
		const uint16_t x = uint16_t(i % HORIZONTAL);
		const uint16_t z = uint16_t(i / HORIZONTAL);
		for (size_t y = 0; y < VERTICAL; y++)
		{
			loadedScene.voxels[x][y][z] = 3;//water
			//loadedScene.voxels[x][y][z] = 0;//air
		}
	}
	return;*/

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	FastNoiseLite noise2;
	noise2.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

	//perlinA + perlinA*cellularA

	for (size_t i = 0; i < (HORIZONTAL * HORIZONTAL); i++)
	{
		const uint16_t x = uint16_t(i % HORIZONTAL);
		const uint16_t z = uint16_t(i / HORIZONTAL);

		const float fx = float(x)*0.01f;
		const float fz = float(z)*0.01f;

		const float perlinA = noise2.GetNoise(fx, fz);
		const float cellularA = noise.GetNoise(fx* 31.0f, fz * 31.0f);

		float h = perlinA + perlinA * cellularA+2.4f;
		h *= h * h*1.88f+8.0f;

		//Material chosenMaterial = 1;//gold
		Material chosenMaterial = 3;//water

		const uint16_t mH = uint16_t(h);

		if (mH > 101)
		{
			for (size_t y = 1; y < 100; y++)
			{
				loadedScene.voxels[x][y][z] = chosenMaterial;
			}


			for (size_t y = 100; y < mH; y++)
			{
				loadedScene.voxels[x][y][z] = 2;//snow
			}
		}
		else
		{
			for (size_t y = 1; y < mH; y++)
			{
				loadedScene.voxels[x][y][z] = chosenMaterial;
			}
		}
		loadedScene.voxels[x][0][z] = 4;//white




	}

}
