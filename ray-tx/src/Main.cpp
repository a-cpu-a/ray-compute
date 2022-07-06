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

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <thread>

#include "Globals.h"
#include "Ray.h"
#include "GenerateScene.h"
#include "tracing/TraceRayBatch.h"
#include "SaveData.h"
#include "Random64.h"
#include "Color.h"

int main()
{
    std::cout << "Hello World!\n";

    generateScene();

    std::array<std::vector<Ray>,THREAD_COUNT*LIGHT_POINTS.size()> outputRays;//add theses all up

    size_t nextVectorIdx = 0;//the actualy size of outputRays, after the threads are done

    uint64_t seed = 1;

    for (const LightPoint& data : LIGHT_POINTS)
    {
        uint32_t raysLeft = data.rayCount;

        const uint32_t maxRaysPerThread = data.rayCount / THREAD_COUNT + ((data.rayCount % THREAD_COUNT) != 0);

        std::array<std::thread, THREAD_COUNT> threads;
        for (size_t i = 0; i < THREAD_COUNT; i++)
        {
            _ASSERT(raysLeft != 0);

            uint32_t rayCount = raysLeft;
            if (raysLeft >= maxRaysPerThread)
            {
                rayCount = maxRaysPerThread;
                raysLeft -= maxRaysPerThread;
            }
            else
            {
                raysLeft = 0;
            }


            std::array<Ray, MAX_LIGHT_RAY_COUNT_PER_THREAD> rayData;
            for (size_t j = 0; j < rayCount; j++)
            {
                Ray& ray = rayData[j];
                ray.pos = data.pos;
                ray.waveLenght = getLightWaveForLight(data, seed);

                bool set = false;

                for (size_t i = 0; i < 10000; i++)
                {
                    const glm::i16vec3 intVec = randI16Vec3(seed);
                    if (intVec==glm::i16vec3(0))
                        continue;
                    const glm::vec3 vec = glm::vec3(intVec) / float(INT16_MAX);

                    const float len = glm::length(vec);

                    if (len <= 1.000001f)
                    {
                        ray.dir = glm::i16vec3((vec / len) * float(INT16_MAX));

                        set = true;
                        break;
                    }
                }
                if (!set)
                {
                    ray.dir = glm::i16vec3(INT16_MAX,0,0);
                }
            }

            threads[i] = std::thread(traceRayBatch, rayData, rayCount, &outputRays[nextVectorIdx++]);


        }
        for (size_t i = 0; i < THREAD_COUNT; i++)
        {
            threads[i].join();
        }
    }




    std::vector<Ray> saveDataRays;

    for (size_t i = 0; i < nextVectorIdx; i++)
    {
        const auto& arr = outputRays[i];

        saveDataRays.insert(saveDataRays.end(), arr.begin(), arr.end());
    }
    save("out/water_world.rays", &saveDataRays);

    std::ofstream file("out/suse.txt", std::ios::out);

    for (const Ray& ray : saveDataRays)
    {
        const glm::vec3 color = glm::max(waveLength2RGB(float(ray.waveLenght) / 54.6125f),0.0f);

        file << "particle dust ";
        file << std::to_string(color.r) + " ";
        file << std::to_string(color.g) + " ";
        file << std::to_string(color.b) + " ";
        file << "0.5 ";
        file << std::to_string(ray.pos.x / 10.0f) + " ";
        file << std::to_string(ray.pos.y / 10.0f-128.0f) + " ";
        file << std::to_string(ray.pos.z / 10.0f);
        file << " 0 0 0 0 0 force\n";
        //file << std::to_string(ray.pos.x) + " " + std::to_string(ray.pos.y) + " " + std::to_string(ray.pos.z) + "\n";
    }


    file.close();

    return 0;

    //sizeof(Ray);
    //sizeof(std::array<Ray,2>);

    //std::vector<Ray> aa;
    //std::cout << "sz: " + std::to_string(aa.max_size()) + "\n";
}

