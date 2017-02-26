#pragma once
#include "glad\glad.h"
#include "Camera.h"
#include "EngineUtils.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include "Object.h"
#include "Shader.h"
#include "District.h"

class Engine {
public:
	Engine();
	Camera* camera;
	list<Object*> objects;

	int program_handle;

	int supersampling = 1;

	int width = 10;
	int height = 10;

	int u_MVP;
	int a_Position;

	float forward_speed = 2.8f;
	float backward_speed = 1.8f;
	float strafe_speed = 2.2f;

	bool request_up = false;
	bool request_down = false;
	bool request_left = false;
	bool request_right = false;
	bool request_forward = false;
	bool request_backward = false;

	void init();
	void compile_shaders();
	void create_world();
	void resize(int w,int h);
	void draw(float dT);
	void draw_scene();
	void post_draw();
	void add_object(Object* o);
	static void check_gl_error(const char* tag);
private:
	float speed_up = 0;
	float speed_forward = 0;
	float speed_right = 0;
};

class SH {
public:
	static int bind_shader_program(Engine* e,int program);
	static int load_shaders(char* v, char* f);
	static inline int use_shader_program(int program);
};

class EU {
public:
	static int init_utils(Engine* e);
	static string load_string(char* filename);
	static void genVBOandVAO(float arr[], int len, int &vertVAO);
	static void genFBO(unsigned int* mFB,int width, int height);
	static void genFBODepthmap(unsigned int* handle, int width, int height, unsigned precision);
	static void genFBOTexture(unsigned int* handle,int width, int height,unsigned attachment,int min_f,int max_f);
	static void glUseAttachments(unsigned attachments[]);
	static void render_objects_compute(Engine* e);
	static void create_objects(Engine* e);
};
