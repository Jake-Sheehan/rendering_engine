/*
* mesh.cpp
* This file contains implementations for mesh objects
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#include "mesh.h"

namespace mesh
{

	// Mesh class constructor
	Mesh::Mesh(std::vector<Vertex> uVertices, std::vector<GLuint> uIndices, std::vector<Texture> uTextures)
	{
		vertices = uVertices;
		indices = uIndices;
		textures = uTextures;
		setup();
	}

	void Mesh::setup()
	{
		// Generates vertex buffer objects, vertex array objects, and texture object
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

		glBindVertexArray(0);
	}

	void Mesh::draw(shaders::Shader& shader)
	{
		shader.use();
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (size_t i = 0; i < textures.size(); i++)
		{
			// Activates the texture 
			glActiveTexture(GL_TEXTURE0 + i);
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			// Sets uniform variable in shader
			glUniform1f(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}