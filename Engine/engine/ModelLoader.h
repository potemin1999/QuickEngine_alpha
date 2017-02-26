#pragma once

#include <string>
#include <fstream>
#include <list>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include "Engine.h"

using namespace std;

class ModelLoader {

private:

static void chop(string &str){
	string spaces(" \t\f\v\n\r");
	unsigned int found = str.find_last_not_of(spaces);
	if (found != string::npos) {
		str.erase(found + 1);
	}else {
		str.clear();
	}
}

static void split(string &str, char d,string* tokens) {
	stringstream iss(str);
	string item;
	int i = 0;
	
	while (getline(iss, item, d)) {
		if (item != " " && !item.empty()) {
			chop(item);
			tokens[i] = item;
			i++;
		}
	}
}

public:

static void load_OBJ(char* path, int &vao, int &vertex_count) {

	vertex_count = 0;
	ifstream file(path);
	if (!file) {
		printf("Couldn't open file \"%s\"\n", path);
		return;
	}
	struct vert {
		float xyz[3];
	};
	struct tex {
		float uv[2];
	};
	struct norm {
		float xyz[3];
	};
	vector<vert> vertices;
	vector<tex>  textures;
	vector<norm> normales;

	vector<float> out_array;
	string line;


	while (!file.eof()) {
		getline(file, line);
		if (line.find("#") == 0) {

		}else if (line.find("vt") == 0) {

			string tokens[3];
			split(line, ' ', tokens);
			tex t;
			t.uv[0] = strtof(tokens[1].c_str(), 0);
			t.uv[1] = strtof(tokens[2].c_str(), 0);
			textures.push_back(t);


		}else if (line.find("vn") == 0) {

			string tokens[4];
			split(line, ' ', tokens);
			norm n;
			n.xyz[0] = strtof(tokens[1].c_str(), 0);
			n.xyz[1] = strtof(tokens[2].c_str(), 0);
			n.xyz[2] = strtof(tokens[3].c_str(), 0);
			normales.push_back(n);

		}else if (line.find("v") == 0) {

			string tokens[4];
			split(line, ' ', tokens);
			vert v;
			v.xyz[0] = strtof(tokens[1].c_str(), 0);
			v.xyz[1] = strtof(tokens[2].c_str(), 0);
			v.xyz[2] = strtof(tokens[3].c_str(), 0);
			vertices.push_back(v);

		}else if (line.find("f") == 0) {

			
			string tokens[4];
			split(line,' ', tokens);
			printf("created polygon\n");
			for (int i = 1; i < 4; i++) {
				
				string items[3];
				
				split(tokens[i], '/', items);
				int in[3];
				in[0] = atoi(items[0].c_str()) - 1;
				in[1] = atoi(items[1].c_str()) - 1;
				in[2] = atoi(items[2].c_str()) - 1;
				vert v = vertices[in[0]];
				tex  t = textures[in[1]];
				norm n = normales[in[2]];
				
				for (int i = 0; i < 3; i++)
					out_array.push_back(v.xyz[i]);
				for (int i = 0; i < 2; i++)
					out_array.push_back(t.uv[i]);
				for (int i = 0; i < 3; i++)
					out_array.push_back(n.xyz[i]);
				vertex_count++;
				printf("    v: %f %f %f t : %f %f n : %f %f %f\n", v.xyz[0], v.xyz[1], v.xyz[2], t.uv[0], t.uv[1], n.xyz[0], n.xyz[1], n.xyz[2]);
			}
			printf("end of polygon\n\n");
		}
	}
	//printf("size of v:%i t:%i n:%i", vertices.size(), textures.size(), normales.size());
	EU::genVBOandVAO(&out_array[0], sizeof(out_array[0]) * out_array.size(), vao);
	printf("model loaded form file %s\n", path);


}


};