#pragma once

#include <glm/glm.hpp>

struct BoxColliderComponent 
{
	int width;
	int heigth;
	glm::vec2 offset;

	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0))
	{
		this->width = width;
		this->heigth = height;
		this->offset = offset;
	}

};