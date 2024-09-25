// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the mesh header. This prevents linking conflicts.
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
using namespace std;
#include "shader_program.h"

// "Vertex" structure contains the minimum of things a vertex needs:
// 1) position;
// 2) normal - vector normal to vertex's surface;
// 3) texture coordinates.
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

// "Texture" structure contains the minimum of things a texture needs:
// 1) id;
// 2) type - diffuse, specular or emission map.
struct Texture
{
	unsigned int id;
	string type;
};

// MESH is a single drawable subentity (submodel, subshape) of the model.
// "Mesh" class is the minimal representation of things needed to draw a mesh in OpenGL:
// 1) vertices;
// 2) indices - array of indexes of the vertices, representing the order in which the vertices should be connected;
// 3) textures.
class Mesh
{
private:
	unsigned int VAO = 0u;
	unsigned int VBO = 0u;
	unsigned int EBO = 0u;

	void setupMesh()
	{
		// Create memory on the GPU where vertex data and index data will be stored.
		// Said data will be handled by VAO and vertex/element buffer objects inside that VAO.
		// Core OpenGL REQUIRES the use of VAOs!
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Bind (assign) the newly created VAO to OpenGL's context.
		glBindVertexArray(VAO);
		// Bind (assign) the newly created VBO to OpenGL's context.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Copy user-defined data into the currently bound buffer.
		// Vertex data is now stored on the graphics card's memory.
		// C++ structures have a sequential memory layout, so we can directly pass a pointer to a list of
		// structures as the buffer's data. That pointer translates perfectly to what "glBufferData" function
		// expects as its third argument.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0u], GL_STATIC_DRAW);
		// Bind (assign) the newly created EBO to OpenGL's context.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// Copy user-defined data into the currently bound buffer.
		// Index data is now stored on the graphics card's memory. -||-
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0u], GL_STATIC_DRAW);

		// Tell OpenGL how it should interpret vertex data, per vertex attribute.
		// Position attribute.
		glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
		// Enable vertex position attribute.
		glEnableVertexAttribArray(0u);
		// Normal vector attribute.
		// Preprocessor directive (macro) "offsetof(s, m)" takes two arguments: structure and name of a variable in
		// the structure. It returns the byte offset of that variable from the start of the structure. That byte
		// offset translates perfectly to what "glVertexAttribPointer" functions expects as its sixth argument.
		glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
		// Enable vertex normal vector attribute.
		glEnableVertexAttribArray(1u);
		// Texture coordinate attribute. -||-
		glVertexAttribPointer(2u, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));
		// Enable vertex texture coordinate attribute.
		glEnableVertexAttribArray(2u);

		// Unbind VBO and VAO for safety reasons. This is not neccessary.
		// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
		// This also means it stores its unbind calls, so
		// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		glBindVertexArray(0u);
	}
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void drawUsing(ShaderProgram& shaderProgram);
};
#endif
