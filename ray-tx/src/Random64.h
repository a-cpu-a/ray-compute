#pragma once

#include <glm/glm.hpp>

inline glm::i16vec3 randI16Vec3(uint64_t& seed)
{
	seed += (seed << 10u);
	seed ^= (seed >> 6u);
	seed += (seed << 3u);
	seed ^= (seed >> 11u);
	seed += (seed << 15u);

	const int16_t x = int16_t(seed >> 48);

	const int16_t y = int16_t(seed >> 32);

	seed += (seed << 10u);
	seed ^= (seed >> 6u);
	seed += (seed << 3u);
	seed ^= (seed >> 11u);
	seed += (seed << 15u);

	const int16_t z = int16_t(seed >> 48);


	return glm::i16vec3(x,y ^ int16_t(seed >> 32),z);
}

inline uint16_t randU16(uint64_t& seed)
{
	seed += (seed << 10u);
	seed ^= (seed >> 6u);
	seed += (seed << 3u);
	seed ^= (seed >> 11u);
	seed += (seed << 15u);

	return uint16_t(seed >> 48);
}