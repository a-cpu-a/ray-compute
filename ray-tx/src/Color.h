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

const float BLUE_POINT = 446.58536585f;
const float GREEN_POINT = 539.75609756f;
const float RED_POINT = 605.8974359f;

//const float SPREAD = -1./(6000799.0);//bell era
const float SPREADR = 1.f / 110.0f;
const float PI = 3.14159265f;

const glm::vec3 lum = glm::vec3(0.2126f, 0.7152f, 0.0722f);
const glm::vec3 lumR = 1.0f / lum;

const float lumConst0 = 96555053.0f / 171026.0f;
const float lumConst1 = 1.0f / 717750000.0f * -85513.0f;
const float lumConst2 = 7567522382953.0f / 9820312920000.0f;

float bell(float x, float p)
{
    //float lumCalcX = x-lumConst0;
    //float lumCalc = lumCalcX*lumCalcX*lumConst1+lumConst2;

    float xPi = (x - p) * PI * SPREADR;//*lumCalc;
    return std::sin(xPi) / xPi * 0.5f + 0.5f;
    //float xP = mod(x-p+110.0,220.0)-110.0;
    //float xPS = xP*xP;
    //return exp(xPS*xPS*SPREAD);
    //return sin((x+p)*SPREAD);
}

glm::vec3 waveLength2RGB(float waveLenght)
{
    float lumCalcX = waveLenght - lumConst0;
    float lumCalc = (lumCalcX * lumCalcX * lumConst1 + lumConst2) * 0.25f + 0.75f;

    glm::vec3 col = glm::vec3(bell(waveLenght, RED_POINT), bell(waveLenght, GREEN_POINT), bell(waveLenght, BLUE_POINT));
    col *= col * col;

    col *= lumCalc;

    return col;
}
