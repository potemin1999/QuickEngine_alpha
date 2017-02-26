#pragma once

#include "glad\glad.h"
#include "glm\glm.hpp"
#include "Material.h"

using namespace glm;

class Object {
public:
	Object() {
		mModelMatrix = mat4(1.0f);
	}
	mat4 mModelMatrix;
	Material mat;
	int vert_count;
	//float* data;
	float xyz[3];
	float frustum_radius = 1.0;
	int data;
	int vbo;
	int tbo;
	int nbo;
	bool to_draw = true;
};
