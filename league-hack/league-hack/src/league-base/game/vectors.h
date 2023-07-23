#pragma once
#include <cmath>

struct vec2
{
	float x, y;

	vec2 operator+(const vec2& v) const {
		return { x + v.x, y + v.y };
	}

	vec2 operator-(const vec2& v) const {
		return { x - v.x, y - v.y };
	}

	vec2 operator*(const float& n) const {
		return { x * n , y * n };
	}

	vec2 operator/(const float& n) const {
		return { x / n, y / n };
	}

	float operator>>(const vec2& v) const {
		return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
	}
};

struct vec3
{
	float x, y, z;

	vec3 operator+(const vec3& v) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	vec3 operator-(const vec3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	vec3 operator*(const float& n) const {
		return { x * n , y * n, z * n };
	}

	vec3 operator/(const float& n) const {
		return { x / n, y / n, z / n };
	}
};