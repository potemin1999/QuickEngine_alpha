#include "Main.h"

#define KEY_ESC 256
#define KEY_SPACE 32
#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68


GLFWwindow* window;
Engine* engine;
bool key_forward = false;
bool key_backward = false;
bool key_right = false;
bool key_left = false;
double xpos, ypos;



void check_gl_error(const char* tag) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("!!! ERROR %s : %i \n",tag, error);
	}
}


void error(int error, const char* description) {
	printf("error : %i \n %s \n", error, description);
}

void resize(GLFWwindow* w, int width, int height) {
	engine->resize(width, height);
}

void key_up(int key) {
	printf("key up : %i\n", key);
	switch (key){
	case KEY_ESC: {
		exit(0xc105ed);
		break;
	}
	case 341: {
		engine->request_down = 0;
		break;
	}
	case KEY_SPACE: {
		engine->request_up = 0;
		//exit(0xdeadbeef);
		break;
	}
	case KEY_W:{
		//engine->forward_speed = 0;
		key_forward = false;
		engine->request_forward = 0;
		break;
	}
	case KEY_A: {
		//engine->strafe_speed = 0;
		key_left = false;
		engine->request_left = 0;
		break;
	}
	case KEY_S: {
		//engine->forward_speed = 0;
		key_backward = false;
		engine->request_backward = 0;
		break;
	}
	case KEY_D: {
		//engine->strafe_speed = 0;
		key_right = false;
		engine->request_right = 0;
		break;
	}
	default:break;
	}
}

void key_down(int key) {
	printf("key down : %i\n", key);
	switch (key) {
	case 341: {
		engine->request_down = 1;
		break;
	}
	case KEY_SPACE:{
		engine->request_up = 1;
		break;
	}
	case KEY_W: {
		if (key_backward)  key_backward = false;
		key_forward = true;
		engine->request_forward = 1;
		//engine->forward_speed = 1;
		break;
	}
	case KEY_A: {
		if (key_right) key_right = false;
		key_left = true;
		engine->request_left = 1;
		//engine->strafe_speed = -1;
		break;
	}
	case KEY_S: {
		if (key_forward)  key_forward = false;
		key_backward = true;
		engine->request_backward = 1;
		//engine->forward_speed = -1;
		break;
	}
	case KEY_D: {
		if (key_left) key_left = false;
		key_right = true;
		engine->request_right = 1;
		//engine->strafe_speed = 1;
		break;
	}
	}
}


void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action){
		case 1:{
			key_down(key);
			break;
		}
		case 0: {
			key_up(key);
			break;
		}
	default:break;
	}
}



void init() {
	engine->init();
	check_gl_error("init");
	engine->resize(window_width, window_height);
	check_gl_error("resize");
	engine->create_world();
	check_gl_error("create world");
	engine->compile_shaders();
	check_gl_error("compile shaders");
	printer_build_font();
	glad_glActiveShaderProgram(0, 0);
}


void loop() {
	float lT = 0;
	float cT = 0;
	float dT = 0;
	while (!glfwWindowShouldClose(window)) {
		dT = (cT = float(glfwGetTime())) - lT;
		lT = cT;
		glfwGetCursorPos(window, &xpos,&ypos);
		engine->camera->rotate_view_right(0.001f * (window_width / 2 - float(xpos)));
		engine->camera->rotate_view_up(0.001f*(window_height / 2 - float(ypos)));
		glfwSetCursorPos(window, window_width / 2, window_height / 2);

		engine->draw(dT);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}








int main(void){
	if (!glfwInit()) {
		return -1;
	}
	engine = new Engine();

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

	GLFWmonitor* monitor;
	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
		window_width = vidmode->width;
		window_height = vidmode->height;
		window = glfwCreateWindow(window_width, window_height, "Engine", monitor, NULL);
		//engine->resize(window_width, window_height);
		//camera->updateProjectionMatrix(float(vidmode->width) / float(vidmode->height));
	}
	else {
		window = glfwCreateWindow(window_width, window_height, "Engine", NULL, NULL);
		//engine->resize(window_width, window_height);
		//camera->updateProjectionMatrix(float(window_width) / float(window_height));
	}
	printf("width : %i \nheight : %i \n",window_width, window_height);

	if (!window) {
		printf("Couldn't create window");
		glfwTerminate();

		return WINDOW_CREATING_ERROR;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("glad is not loaded!\n");
		return -1;
	}else printf("OpenGL %d.%d loaded\n", GLVersion.major, GLVersion.minor);
	if (monitor)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, resize);
	glfwSetKeyCallback(window, key);
	glfwSetErrorCallback(error);
	glfwShowWindow(window);
	if (lock_mouse)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	try {
		init();
		loop();
	}catch(exception e){

	}
	

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

