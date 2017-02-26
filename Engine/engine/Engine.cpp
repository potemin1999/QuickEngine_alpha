#include "Engine.h"
#include "EngineUtils.h"


float vert[] =
{-1,-1, 0,     -1, 1, 0,    1, 1, 0,
  1, 1, 0,      1,-1, 0,   -1,-1, 0};

//int indices[] = { 0,1,2,     2,3,0 };

unsigned int* mPostVAO = new unsigned int;
unsigned int* mPostVBO_vert = new unsigned;
unsigned int* mPostVBO_ind = new unsigned;

unsigned int* mFBO = new unsigned;
unsigned int* mFBO_depthmap = new unsigned;
unsigned int* mFBO_textures = new unsigned[3];
int *m_MainPassProgram = new int;
int *m_PostPassProgram = new int;
unsigned int* u_PizelSize = new unsigned;
float* v_PixelSize = new float[2];
unsigned int* u_ScreenSize = new unsigned;
float* v_ScreenSize = new float[2];
unsigned int* u_SuperSampling = new unsigned;
unsigned int* u_Vignette = new unsigned;

void Engine::check_gl_error(const char* tag) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("!!! ERROR %s : %i \n", tag, error);
	}
}

Engine::Engine(){
	printf("created new engine\n");
}

void Engine::init() {
	int i_eu = EU::init_utils(this);
	printf("init engine utils %i \n",i_eu);
	camera = new Camera();
	camera->farZ = 10.0f;
	camera->nearZ = 0.01f;
	camera->fovy = 30.14159f/3.0f;
	float p[] = { 0.0f,0.0f,0.0f };
	float v[] = { 0.0f,0.0f,1.0f };
	float u[] = { 0.0f,1.0f,0.0f };
	camera->position_camera(p,v,u);
	
}


void Engine::compile_shaders() {
	*m_MainPassProgram = SH::load_shaders("shaders\\shader_vertex.glsl","shaders\\shader_fragment.glsl");
	printf("init base shader program %i\n", *m_MainPassProgram);
	SH::bind_shader_program(this, *m_MainPassProgram);
	//*m_MainPassProgram = program_handle;
	printf("main shader program binded");
	
	*m_PostPassProgram = SH::load_shaders("shaders\\post_shader_vertex.glsl", "shaders\\post_shader_fragment.glsl");
	printf("init post shader program %i\n", *m_PostPassProgram);
	*u_PizelSize = glGetUniformLocation(*m_PostPassProgram, "u_PixelSize");
	*u_ScreenSize = glGetUniformLocation(*m_PostPassProgram, "u_ScreenSize");
	*u_SuperSampling = glGetUniformLocation(*m_PostPassProgram, "u_SuperSampling");
	*u_Vignette = glGetUniformLocation(*m_PostPassProgram, "u_Vignette");
	//SH::bind_shader_program(this, *m_PostPassProgram);
	printf("post shader program binded");
}

void Engine::create_world() {
	EU::create_objects(this);
}

void Engine::resize(int w,int h) {

	
	if (h == 0) h = 10;
	if (w == 0) w = 10;
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	if (supersampling != 1) {
		w *= supersampling; h *= supersampling;
	}
	camera->set_perspective(((float)w) / ((float)h), camera->fovy, camera->farZ);
	EU::genFBO(mFBO, w, h);
	int m_offset = sizeof(unsigned);

	EU::genFBODepthmap(mFBO_depthmap, w, h, GL_DEPTH_COMPONENT32F);
	v_PixelSize[0] = 1.0f / ((float)(w));
	v_PixelSize[1] = 1.0f / ((float)(h));
	v_ScreenSize[0] = (float)w;
	v_ScreenSize[1] = (float)h;
	EU::genFBOTexture(mFBO_textures + m_offset * 0, w, h, GL_COLOR_ATTACHMENT0, GL_LINEAR, GL_NEAREST);
	EU::genFBOTexture(mFBO_textures + m_offset * 1, w, h, GL_COLOR_ATTACHMENT1, GL_LINEAR, GL_NEAREST);
	EU::genFBOTexture(mFBO_textures + m_offset * 2, w, h, GL_COLOR_ATTACHMENT2, GL_LINEAR, GL_NEAREST);
	if (supersampling != 1) {
		w /= supersampling; h /= supersampling;
	}
	EU::glUseAttachments(new unsigned[3]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });

	unsigned int post_vbo;
	glGenBuffers(1,&post_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vert, GL_STATIC_DRAW);


	glGenVertexArrays(1, mPostVAO);
	glBindVertexArray(*mPostVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	
	//EU::genVBOandVAO(vert, 12 * sizeof(float), *mPostVAO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	
}

void Engine::draw(float dT) {
	if (request_forward&!request_backward) camera->move_camera(forward_speed*dT);
	if (request_backward &!request_forward) camera->move_camera(-backward_speed*dT);
	if (request_right & !request_left) camera->move_strafe(strafe_speed*dT);
	if (request_left & !request_right) camera->move_strafe(-strafe_speed*dT);
	if (request_up & !request_down) camera->move_camera_up( dT);
	if (request_down & !request_up) camera->move_camera_up(-dT);
	camera->look();

	if (supersampling!=1)
	  glViewport(0, 0, supersampling*width, supersampling*height);
	glBindFramebuffer(GL_FRAMEBUFFER, *mFBO);
	draw_scene();
	//glFlush();

	if (supersampling!=1)
	  glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	post_draw();
	//draw_scene();
	glFlush();
}

void Engine::post_draw(){
	glUseProgram(*m_PostPassProgram);
	//glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *(mFBO_textures + 0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *(mFBO_textures + 1));

	glBindVertexArray(*mPostVAO);
	glEnableVertexAttribArray(0);
	glUniform2f(*u_PizelSize, v_PixelSize[0], v_PixelSize[1]);
	glUniform2f(*u_ScreenSize,v_ScreenSize[0],v_ScreenSize[1]);
	glUniform1i(*u_SuperSampling, supersampling);
	glUniform1i(*u_Vignette,1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, mFBO_textures[1]);

	glDrawArrays(GL_TRIANGLES, 0,6);

	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Engine::draw_scene() {
	//check_gl_error("pre draw");
	glUseProgram(*m_MainPassProgram);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//check_gl_error("draw");
	for each (Object* o in objects){
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, o->data);
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(o->data);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glUniformMatrix4fv(u_MVP, 1, GL_FALSE,camera->value_ptr( o->mModelMatrix*camera->mMVPMatrix));
		glActiveTexture(GL_TEXTURE0);
		//textureManager->BindTexture(texID);
		glDrawArrays(GL_TRIANGLES, 0, o->vert_count);
		//printf("vert count %i\n", o->vert_count);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}
	//check_gl_error("post draw");
	
}

void Engine::add_object(Object* o) {
	objects.push_back(o);
}

