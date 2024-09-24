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

	void setupMesh();
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
