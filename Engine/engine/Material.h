#pragma once
#include "glad\glad.h"

struct Material {
	unsigned int tex;
};

class MH {
public:
	Material create_material_from_file(char* path) {
		Material m;
		m.tex = 0;
		return m;
	}
};
