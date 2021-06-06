#pragma once
#include "Components.h"
#include "game.h"
class TransformComponent : public Component
{
public:
	VECTOR2 position;
	VECTOR2 center_position;
	int height;
	int width;
	float scale = 1.0;

	TransformComponent(float x, float y, int w, int h, float sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{

	}
	void update(const float& frametime) override
	{

	}
	void updatePosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
};