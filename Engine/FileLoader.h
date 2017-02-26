#pragma once

#include <fstream>
#include <string>
#include <direct.h>
#include "engine\Object.h"


using namespace std;


string loadFile(string filename) {
	char path_c[1024];
	_getcwd(path_c, sizeof(path_c));
	string path = string(path_c);
	path.append("\\");
	path.append(filename);
	ifstream file(path);
	if (!file.is_open()) {
		printf("Error loading file %s\n", path.data());
		return "";
	}
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	printf("Loaded shader %s\n", path.data());
	return str;
}
