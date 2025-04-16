#pragma once


class Box {
public:
	float x, y;
	int ID;
	int type;

	Box() {}
	Box(int _ID, float _x, float _y, int _type)
		: ID(_ID), x(_x), y(_y), type(_type)
	{}
};