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

#include "WaveLenghtInSegment.h"
#include "../Globals.h"
#include "../Ray.h"


struct TracedRay
{
    Ray data;
    bool save = false;
};

const glm::vec3 WORLD_SIZE = glm::vec3(HORIZONTAL, VERTICAL, HORIZONTAL);


inline uint8_t rand8(uint16_t& seed)
{
    seed = (seed << 4 | seed >> 12) ^ 0x2A5F;
    seed = (seed << 5 | seed >> 11) ^ 0x1ECD;

    return (seed&0xFF)^0x2A;
}

//returns if it should save the data
TracedRay traceRay(Ray rayData, uint16_t& seed)
{
    bool run = true;

    TracedRay ret;

    uint8_t prevRefraction = 0;
    float prevRefractionIdx = 1.0f;

    while (run)
    {

        const glm::vec3 rayDir = glm::vec3(rayData.dir) * 1.42f;
        glm::vec3 voxelPos = glm::floor(rayData.pos);//voxel floor pos
        const glm::vec3 RayDirInverse = 1.0f / rayDir;
        const glm::vec3 rayDirSign = glm::sign(rayDir);
        const glm::vec3 delta = glm::min(RayDirInverse * rayDirSign, 1.0f);
        // start at intersection of ray with initial cell
        glm::vec3 rayLen = glm::abs((voxelPos + glm::max(rayDirSign, 0.0f) - rayData.pos) * RayDirInverse);

        for (size_t i = 0; i < 92688; ++i) {//max ~= 2^16 * (2^0.5)




            const float next_t = glm::min(rayLen.x, glm::min(rayLen.y, rayLen.z));
            //if (next_t > 1.0f) //no cuz i want infinite looping im some cases
            //    break;

            //edgePos = rayData.pos + next_t * rayDir

            const glm::vec3 cmp = glm::step(rayLen, glm::vec3(rayLen.y, rayLen.z, rayLen.x));
            rayLen += delta * cmp;
            voxelPos += rayDirSign * cmp;


            if (glm::any(glm::lessThan(voxelPos, glm::vec3(0.0f))) || glm::any(glm::lessThanEqual(WORLD_SIZE,voxelPos)))
            {
                ret.save = true;
                rayData.pos = rayData.pos + next_t * rayDir;
                ret.data = rayData;
                run = false;
                break;
            }

            const glm::vec3 normal = -(cmp * rayDirSign);

            const glm::u16vec3 fPos = voxelPos;

            const Material& voxelMaterial = loadedScene.voxels[fPos.x][fPos.y][fPos.z];

            const MaterialData& voxelMaterialData = matrialList[(uint8_t)voxelMaterial];

            //check absorbtion stuff
            if (127 > rand8(seed))//50% absorbtion chance
            {
                if (waveLenghtInSpectrumSegments(rayData.waveLenght, voxelMaterialData.absorbtionData) != UINT16_MAX)
                {
                    //might be a good idea to ... absorb

                    rayData.waveLenght *= 2;//big = less power

                    if (waveLenghtInSpectrumSegments(rayData.waveLenght, voxelMaterialData.absorbtionData) != UINT16_MAX)
                    {
                        //might be a good idea to ... absorb
                        ret.save = false;
                        run = false;
                        break;
                    }
                    else
                    {
                        //randomize dir stuff & make new ray thing
                        rayData.pos = rayData.pos + next_t * rayDir;
                        break;
                    }
                }
            }
            //only when not absorbed, or deenergized VVV


            {
                const uint16_t idx = waveLenghtInSpectrumSegments(rayData.waveLenght,voxelMaterialData.transparencyData);
                if (idx != UINT16_MAX)
                {
                    //its translucent! :D
                    const uint8_t refraction = voxelMaterialData.transparencySegmentInfo[idx];
                    const float refractionIdx = (float(refraction) / 64.0f + 1.0f) / (float(rayData.waveLenght) / 54.6125f);
                    if (prevRefraction != refraction)
                    {
                        //math

                        /*const float ratio = prevRefractionIdx / refractionIdx;
                        const float dotProduct = glm::dot(glm::normalize(rayDir), normal);

                        const float magic = 1.0 - ratio * ratio * (1.0 - dotProduct * dotProduct);

                        if (magic < 0.0f)
                        {
                            //internal reflection?
                            rayData.dir = glm::i16vec3(glm::reflect(glm::vec3(rayData.dir), normal));
                            rayData.pos = rayData.pos + next_t * rayDir;
                            break;

                        }
                        rayData.dir = glm::i16vec3(glm::reflect(glm::vec3(rayData.dir), normal));
                        rayData.pos = rayData.pos + next_t * rayDir;*/

                        const glm::vec3 val = glm::refract(glm::normalize(rayDir), normal, prevRefractionIdx / refractionIdx);

                        rayData.pos = rayData.pos + next_t * rayDir;

                        if (val == glm::vec3(0))
                        {
                            //total internal reflection
                            rayData.dir = glm::i16vec3(glm::reflect(glm::vec3(rayData.dir), normal));
                            //rayData.pos = rayData.pos + next_t * rayDir;
                        }
                        else
                        {
                            rayData.dir = glm::i16vec3(val * float(INT16_MAX));
                        }


                        prevRefraction = refraction;
                        prevRefractionIdx = refractionIdx;
                        break;
                    }
                    else
                    {
                        continue;//skip reoughness
                    }
                }
            }
            //only for solid meterials VVVV

            //roughness
            if (voxelMaterialData.roughness > rand8(seed))
            {
                //randomize dir stuff | AWAY FROM SURFACE!
                rayData.pos = rayData.pos + next_t * rayDir;
                break;
            }
            else
            {
                rayData.dir = glm::i16vec3(glm::reflect(glm::vec3(rayData.dir), normal));
                rayData.pos = rayData.pos + next_t * rayDir;
                break;
                //reflect
            }

        }
    }
    return ret;
}
