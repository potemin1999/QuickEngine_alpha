#define ENGINE_UTILS_INIT_SUCCEED 0x00000000
#include <string>
#include <direct.h>
#include <fstream>
#include "Engine.h"
#include "glad\glad.h"
#include "TestModel.h"


int EU::init_utils(Engine* e) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0, 0, 1, 1);

	return ENGINE_UTILS_INIT_SUCCEED;
}



string EU::load_string(char* filename) {
	char path[1024];
	_getcwd(path, sizeof(path));
	string _path = string(path);
	_path.append("\\");
	_path.append(filename);
	ifstream file(_path);
	if (!file.is_open()) {
		printf("Error loading file %s\n", _path.data());
		return "";
	}
	std::string str((std::istreambuf_iterator<char>(file)),   std::istreambuf_iterator<char>());
	printf("read file %s\n", _path.data());
	//printf("FILE CONTENT : %s",str.c_str());
	return str;
}



struct Vertex {
	float position[3];
	float uv[2];
	float normal[3];
};

void EU::genVBOandVAO(float arr[], int size, int &verts) {
	unsigned int vbo1;
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, size, arr, GL_STATIC_DRAW);
	unsigned int vao1;
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glBindVertexArray(0);
	Engine::check_gl_error("get vbo and vao");
	verts = vao1;
}

void EU::genFBO(unsigned int* mFB, int width, int height) {
	glGenFramebuffers(1, mFB);
	glBindFramebuffer(GL_FRAMEBUFFER, *mFB);
}

void EU::genFBODepthmap(unsigned int* handle, int width, int height, unsigned precision) {
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *handle, 0);
}

void EU::genFBOTexture(unsigned int* handle, int width, int height, unsigned attachment, int min_f, int max_f) {
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_f);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *handle, 0);
}

void EU::glUseAttachments(unsigned attachments[]){
	glDrawBuffers(sizeof(attachments) / sizeof(attachments[0]) , attachments);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EU::render_objects_compute(Engine* e) {
	for each(Object* o in e->objects) {
		float length2 = 0;
	}
}

void EU::create_objects(Engine* e) {
	TestModel* tm = new TestModel();
	e->add_object(tm);
}
