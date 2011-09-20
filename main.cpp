#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL/SDL.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "sys/time.h"

using namespace std;

GLuint vert_shader_id;
GLuint frag_shader_id;
GLuint shader_program_id;
GLuint time_id;

float current_time = 0.0;

const int W_WIDTH = 800;
const int W_HEIGHT = 600;

bool loadShaderSource(const string& filename, string &source){
	std::ifstream file;
	file.open(filename.c_str());
	if(!file){
	return false;
	}
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	source = stream.str();

	return true;
}

void GL_Setup(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 1, 1, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void shaderSetup(){
	// Load shader
	string frag;
	string vert;
	if(loadShaderSource("sin.vs", vert) && loadShaderSource("sin.fs", frag)){
		const char* frag_source = frag.c_str();
		const char* vert_source = vert.c_str();

		vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
		frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

		int status;

		glShaderSource(vert_shader_id, 1, &vert_source, NULL);
		glCompileShader(vert_shader_id);
		glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE){
		  int logLength;
		  GLcharARB *log;
		  log = new GLcharARB[1024];

		  glGetInfoLogARB(shader_program_id, 1024, &logLength, log);

		  if(logLength > 0){
			cout << "Shader info: " << log << endl;
			exit(-1);
		  }
		}

		glShaderSource(frag_shader_id, 1, &frag_source, NULL);
		glCompileShader(frag_shader_id);
		glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE){
		  int logLength;
		  GLcharARB *log;
		  log = new GLcharARB[1024];

		  glGetInfoLogARB(shader_program_id, 1024, &logLength, log);

		  if(logLength > 0){
			cout << "Shader info: " << log << endl;
			exit(-1);
		  }
		}

		shader_program_id = glCreateProgram();
		glAttachShader(shader_program_id, vert_shader_id);
		glAttachShader(shader_program_id, frag_shader_id);

		glLinkProgram(shader_program_id);

		int logLength;
		GLcharARB *log;
		log = new GLcharARB[1024];

		glGetProgramInfoLog(shader_program_id, 1024, &logLength, log);

		if(logLength > 0){
			cout << "Program info: " << log << endl;
			exit(-1);
		}

		GLuint resolution_id = glGetUniformLocation(shader_program_id, "resolution");
		time_id = glGetUniformLocation(shader_program_id, "time");

		glUseProgram(shader_program_id);

		glUniform2f(resolution_id, (float)W_WIDTH, (float)W_HEIGHT);
	} else {
		cout << "Could not load shader." << endl;
		exit(-1);
	}
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(1.0, 0.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(0.0, 1.0);
	glEnd();
}

bool events(){
    SDL_Event event;
    int keycode;
    if(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_KEYDOWN:
              keycode = event.key.keysym.sym;
              switch(keycode){
                case SDLK_q:
                  return false;
                  break;
              }
              break;
            case SDL_QUIT:
              return false;
              break;
        }
    }
    return true;
}

void main_loop_function(){
	while(events()){
		glUniform1f(time_id, current_time);
		current_time += 0.01;

		draw();
		SDL_GL_SwapBuffers();
	}
}

int main(int argc, char** argv){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
	}
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int vidFlags = SDL_OPENGL;
	if(info->hw_available){
		vidFlags |= SDL_HWSURFACE;
	} else {
		vidFlags |= SDL_SWSURFACE;
	}
	int bpp = info->vfmt->BitsPerPixel;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	SDL_SetVideoMode(W_WIDTH, W_HEIGHT, bpp, vidFlags);

	glewInit();
	GL_Setup(W_WIDTH, W_HEIGHT);
	shaderSetup();
	glutInit(&argc, argv);

	main_loop_function();

	return 0;
}
