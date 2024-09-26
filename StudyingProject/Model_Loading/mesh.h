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
// 2) type - diffuse, specular or emission map;
// 3) path - local path of texture image file (we make the assumption that the model and its textures are located
//           in the same directory).
struct Texture
{
	unsigned int id;
	string type;
	string path;
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

	// Before rendering the mesh, we first want to bind the appropriate textures. However, we don't know how many
	// textures the mesh has and of what type, nor does the mesh have any textures at all. In order to set the
	// texture units and the samplers in the shader program, we need to get creative.
	// One way of solving this problem is to assume a certain naming convention. Each diffuse map will be named
	// "texture_diffuseN", each specular map will be named "texture_specularN" and each emission map will be named
	// "texture_emissionN". Number "N" is from set {1, ..., maximum number of texture samplers allowed}. By using
	// this naming convention, we can process any amount of textures on a single mesh and the shader developer is
	// free to use as many textures as they want by defining the proper texture samplers.
	void drawUsing(ShaderProgram& shaderProgram)
	{
		unsigned int diffuseMapNumber = 1u;
		unsigned int specularMapNumber = 1u;
		unsigned int emissionMapNumber = 1u;
		for (unsigned int i = 0u; i < textures.size(); i++)
		{
			// Retrieve the texture number ("N" in "{texture_type}N").
			string typeOfTexture = textures[i].type;
			string number = "";
			if (typeOfTexture == "texture_diffuse")
			{
				number = to_string(++diffuseMapNumber);
			}
			else if (typeOfTexture == "texture_specular")
			{
				number = to_string(++specularMapNumber);
			}
			else if (typeOfTexture == "texture_emission")
			{
				number = to_string(++emissionMapNumber);
			}
			// Tell OpenGL to which texture unit the shader sampler belongs to, by setting the shader sampler.
			// Prefix "material." has to be added because texture samplers are fields of "Material" structure.
			shaderProgram.setIntegerUniform(("material." + typeOfTexture + number).c_str(), i);

			// Activate texture unit (one of 16). After activating a texture unit, a subsequent "glBindTexture"
			// call will bind that texture to the currently active texture unit. Texture unit "GL_TEXTURE0" is
			// always active by default, so it isn't necessary to manually activate any texture unit if only one
			// texture is used (like in examples previous to "Textures, combined").
			glActiveTexture(GL_TEXTURE0 + i);
			// Bind (assign) the texture to OpenGL's context.
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// Unbind VAO for safety reasons. This is not neccessary.
		// VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
		// This also means it stores its unbind calls, so
		// DO NOT EVER unbind EBO before unbinding VAO, otherwise it won't have a configured EBO.
		glBindVertexArray(0u);
	}
};
#endif
