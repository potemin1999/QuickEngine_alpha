#pragma once

#include <stdio.h>
#include <tchar.h>


#include "glad.h"
#include "GLFW\glfw3.h"

#include <gl\GLU.h>

#include "FileLoader.h"
#include "engine\Engine.h"
#include "Settings.h"
#include "Printer.h"

#define WINDOW_CREATING_ERROR 0x89089089


int window_width=1900;
int window_height=1000;
int main(void);
void resize(GLFWwindow* w, int width, int height);
void key(GLFWwindow* window, int key, int scancode, int action, int mods);
void error(int error, const char* description);
void init();
void loop();
void main_entry_point();
