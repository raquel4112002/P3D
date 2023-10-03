#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>

#include <GL/glew.h>
#include <Windows.h>
#include <gl\GL.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4, ...
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...
#include <glm/gtc/type_ptr.hpp> // value_ptr


#define WIDTH 1280
#define HEIGHT 720
#define GLEW_STATIC
#define NumeroDeVAOs 1 //1 VAO
#define NumBuffers 3 // Vértices, Coordenadas texturas, normais

class Balls {
public:
	void Read(const std::string objFilepath);
	void Send(void);
	void Load(std::string fileName);
	void Draw(glm::vec3 position, glm::vec3 orientation, glm::mat4 modelMatrix);
	void Texture(const std::string textureFile);
	void GetPointersId(GLuint shader, GLuint coordsId, GLuint normalsId, GLuint texId, GLuint textureId, int counter);
	glm::mat4 ball;
	float accumulatedRotationY = 0.0f;
	float ZOOM = 15.0f;
	glm::vec3 ka, kd, ks;
	GLfloat ns;

	//Balls(const char* filename);
private:
	//Vertex Pos
	std::vector<glm::vec3> vertex_positions;
	std::vector<glm::vec2> vertex_textures_coords;
	std::vector<glm::vec3> vertex_normals;

	
	GLuint VAO;
	GLuint Buffers[NumBuffers];
	GLuint programa;
	GLuint textureName;

	const GLuint NumVertices = 4034;


	GLuint coordsId,
		normalsId,
		texId,
		textureId;
	int counter;


	float Ns; // Expoente especular
	float angle = 0.0f;
	public:

	
};


