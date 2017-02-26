#pragma once

#include "Object.h"
#include "ModelLoader.h"

class TestModel : public Object {
public:
	TestModel() : Object() {
		mat.tex = 0;
		ModelLoader::load_OBJ("models\\floating_crystal.obj", data, vert_count);
	}
};
