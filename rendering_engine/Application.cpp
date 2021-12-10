/*
* Application.cpp
* This file creates a window and draws two triangles
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* 
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>
#include "shaders.h"
#include "mesh.h"
#include "setup.h"
#include "input.h"
#include "colors.h"
#include "model.h"
#include <vector>
#include <chrono>
#include <thread>

int main()
{
	// -------------------- INITIALIZATION --------------------

	// Initializes window

	// Screen size must be hardcoded until I develop a front end GUI
	const unsigned int SCREEN_WIDTH = 960;
	const unsigned int SCREEN_HEIGHT = 540;
	GLfloat aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	GLFWwindow* window = setup::initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// -------------------- SHADER PROGRAMS --------------------
	// Creates light source shader program so that light source will not be effected
	// by ambient light
	const GLchar* vertexShaderPath = "shader_source/light_source_vertex_shader.txt";
	const GLchar* fragmentShaderPath = "shader_source/light_source_fragment_shader.txt";
	shaders::Shader lightSourceShader = shaders::Shader(vertexShaderPath, fragmentShaderPath);

	// Creates object shader program that is effected by ambient light
	vertexShaderPath = "shader_source/vertex_shader.txt";
	fragmentShaderPath = "shader_source/fragment_shader.txt";
	shaders::Shader objectShader = shaders::Shader(vertexShaderPath, fragmentShaderPath);

	// -------------------- Projection --------------------
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		aspect,
		0.1f,
		100.0f);

	// Sets perspective in light source shader
	lightSourceShader.use();
	GLuint lightSourceProjectionLoc = glGetUniformLocation(lightSourceShader.ID, "projection"); // Get projection location
	GLuint lightSourceViewLoc = glGetUniformLocation(lightSourceShader.ID, "view"); // Get view location
	glUniformMatrix4fv(lightSourceProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Sets perspective in object shader
	objectShader.use();
	GLuint projectionLoc = glGetUniformLocation(objectShader.ID, "projection"); // Get projection location
	GLuint viewLoc = glGetUniformLocation(objectShader.ID, "view"); // Get view location
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// -------------------- LIGHTING --------------------

	const char* lightPath = "models/light_source_01/light_source_01.object";
	model::LightSource light = model::LightSource(
		lightPath,                         // path to wavefront object
		objectShader,                      // object shader
		lightSourceShader,                 // light source shader
		glm::vec4{0.3f, 0.3f, 0.3f, 1.0f}, // ambient light RGBA
		glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // diffuse light RGBA
		glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, // specular light RGBA
		1.0f                               // ambient strength
	);
	light.translate(10.0f, 5.5f, 0.0f, objectShader);


	// -------------------- SCENE OBJECTS --------------------

	const char* lampPath = "models/lamp/lamp.object";
	model::Model lamp = model::Model(lampPath);
	lamp.translate(12.0f, 0.25f, 0.0f);
	lamp.rotate(90.0f, 'y');

	const char* tablePath = "models/table/table.object";
	model::Model table = model::Model(tablePath);
	table.scale(4.0f, 4.0f, 4.0f);

	const char* bookPath = "models/book/book.object";
	model::Model book = model::Model(bookPath);
	book.translate(0.0f, 0.0f, 10.0f);
	book.rotate(-90.0f, 'y');

	const char* headphonesPath = "models/headphones/headphones.object";
	model::Model headphones = model::Model(headphonesPath);
	headphones.translate(5.0f, 0.5f, 6.0f);
	headphones.rotate(225.0f, 'y');

	const char* penPath = "models/pen/pen.object";
	model::Model pen = model::Model(penPath);
	pen.translate(-2.0f, 0.25f, 11.0f);

	const char* cupPath = "models/cup/cup.object";
	model::Model cup = model::Model(cupPath);
	cup.translate(-5.0f, 0.1f, 5.0f);
	cup.rotate(180.0f, 'y');

	// ~~~~~~~~~~~~~~~~~~~~ RENDER LOOP ~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	while (!glfwWindowShouldClose(window))
	{
		// -------------------- RENDER --------------------
	
		// Enable Z-depth testing to test which objects are covered by others
		glEnable(GL_DEPTH_TEST);
		// Accept the closer fragment
		glDepthFunc(GL_LESS);
		
		// Clears frame and Z buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Sends view informtion to uniform variables in shaders
		lightSourceShader.use();
		glUniformMatrix4fv(lightSourceViewLoc, 1, GL_FALSE, glm::value_ptr(input::view)); // updates view in light shader
		objectShader.use();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(input::view));  // updates view in object shader
		GLuint viewPosLoc = glGetUniformLocation(objectShader.ID, "viewPos");
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(input::cameraPos)); // updates view position
		
		// Draws shapes
		light.draw(lightSourceShader);
		lamp.draw(objectShader);
		table.draw(objectShader);
		book.draw(objectShader);
		headphones.draw(objectShader);
		pen.draw(objectShader);
		cup.draw(objectShader);

		// Swaps front and back buffer
		glfwSwapBuffers(window);

		// -------------------- HANDLE INPUT --------------------
		glfwPollEvents();

	}

	// Frees allocated resources used by GLFW
	glfwTerminate();
	return 0;
}