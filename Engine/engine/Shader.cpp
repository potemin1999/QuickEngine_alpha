#include "glad\glad.h"
#include "Engine.h"
#include <string.h>


int load_vertex_shader(char* v) {
	int vertex_handle = glCreateShader(GL_VERTEX_SHADER);
	if (vertex_handle != 0) {
		string s = EU::load_string(v);
		const char* c = s.c_str();
		//printf("\n vertex shader: \n %s \n end of vertex shader \n",c);
		glShaderSource(vertex_handle,1,&c,nullptr);
		glCompileShader(vertex_handle);
		int* status = new int[1];
		glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, status);
		if (status[0] != 0) {
			printf("vertex shader compiled succeed to link %i from file %s \n",vertex_handle, v);
		}else {
			char log[1024];
			int* l = 0;
			glGetShaderInfoLog(vertex_handle, 1024, l, log);
			printf("vertex shader compile failed : %s\n",log);
			glDeleteShader(vertex_handle);
		}
	}else {
		printf("vertex handle NULL");
	}
	return  vertex_handle;
}

int load_fragment_shader(char* f) {
	int fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragment_handle != 0) {
		string s = EU::load_string(f);
		const char* c = s.c_str();
		//printf("%s", c);
		glShaderSource(fragment_handle, 1, &c, nullptr);
		glCompileShader(fragment_handle);
		int* status = new int[1];
		glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, status);
		if (status[0] != 0) {
			printf("fragment shader compiled succeed to link %i from file %s \n",fragment_handle, f);
		}else {
			char log[1024];
			int* l = 0;
			glGetShaderInfoLog(fragment_handle, 1024,l, log);
			printf("fragment shader compile failed : %s\n",log);
			glDeleteShader(fragment_handle);
		}
	}else {
		printf("fragment handle NULL");
	}
	return fragment_handle;
}



int SH::load_shaders(char* v, char* f) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("before program creting error %i: \n", error);
	}
	int program_handle = glCreateProgram();
	if (program_handle != 0) {
		int vertex_handle = load_vertex_shader(v);
		int fragment_shader = load_fragment_shader(f);
		glAttachShader(program_handle,vertex_handle);
		glAttachShader(program_handle, fragment_shader);
		glLinkProgram(program_handle);
		int* status = new int[1];
		glGetProgramiv(program_handle, GL_LINK_STATUS, status);
		if (status[0] != 0) {
			printf("shader program linked succeed to link %i\n", program_handle);
			glUseProgram(program_handle);
		}else {
			char log[1024];
			int* l = 0;
			glGetProgramInfoLog(program_handle, 1024, l, log);
			printf("shader program linking failed : %s\n",log);
			glDeleteProgram(program_handle);
		}
		glReleaseShaderCompiler();
	}else {
		printf("program handle NULL");
	}

	return program_handle;
}

int SH::bind_shader_program(Engine* e, int program) {
	e->u_MVP = glGetUniformLocation(program, "u_MVP");
	//e->a_Position = glGetAttribLocation(program, "a_Position");
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("bind shader error %i: \n", error);
	}
	return 0;
}

int SH::use_shader_program(int program) {
	glUseProgram(program);
}