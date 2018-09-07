#pragma once


#define SHAPE_EVENT_HOOVER 1 << 4
#define SHAPE_EVENT_HOOVER 1 << 5

#define SHAPE_RECTANGLE 1
#define SHAPE_OVAL 2


struct Shape
{
	uint32_t type;
	uint32_t flags;
	float p [2];
	float l [2];
};


struct Rectangle
{
	struct Shape shape;
	
};
