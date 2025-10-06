#pragma once

#ifndef MY_STRUCTS_H
#define MY_STRUCTS_H

#include <Windows.h>

//-----------------------My Structs-----------------------------

struct Vector2
{
	int x;
	int y;

	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 right;
	static const Vector2 up;

	Vector2(int x = 0, int y = 0) : x(x), y(y) {}

	Vector2 operator+(const Vector2& other) const {
		return (x + other.x, y + other.y);
	}
	Vector2 operator+=(const Vector2& other) const {
		return (x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other) const {
		return (x - other.x, y - other.y);
	}
	Vector2 operator-=(const Vector2& other) const {
		return (x - other.x, y - other.y);
	}
	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vector2& other) const {
		return x != other.x || y != other.y;
	}
};

struct Vector2f
{
	float x;
	float y;

	static const Vector2f zero;
	static const Vector2f one;
	static const Vector2f right;
	static const Vector2f up;

	Vector2f (int x = 0, int y = 0) : x(x), y(y) {}

	Vector2f operator+(const Vector2f& other) const {
		return Vector2f(x + other.x, y + other.y);
	}
	Vector2f operator+=(const Vector2f& other) const {
		return Vector2f(x + other.x, y + other.y);
	}
	Vector2f operator-(const Vector2f& other) const {
		return Vector2f(x - other.x, y - other.y);
	}
	Vector2f operator-=(const Vector2f& other) const {
		return Vector2f(x - other.x, y - other.y);
	}
	bool operator==(const Vector2f& other) const {
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vector2f& other) const {
		return x != other.x || y != other.y;
	}
};

#endif