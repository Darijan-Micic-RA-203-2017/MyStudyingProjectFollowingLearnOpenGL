// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the model header. This prevents linking conflicts.
#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
using namespace std;
#include "shader_program.h"
#include "mesh.h"

class Model
{
private:
	vector<Mesh> meshes;
	string directory;

	// Use the Assimp library to load the model into the Assimp's data structure. The root object of the Assimp's
	// data structure is the "SCENE" object. Once we have the scene, we can access ALL THE DATA we need from the
	// loaded model.
	void loadModelFrom(string path)
	{
		Assimp::Importer modelImporter;
		// "ReadFile" method of Assimp's "Importer" class expects post-processing options as its second argument.
		// "aiProcess_Triangulate" - tells Assimp that if the model doesn't entirely consist of triangles, it
		// should transform all the model's primitive shapes to triangles first.
		// "aiProcess_FlipUVs"     - tells Assimp to flip the texture coordinates on the y-axis where necessary.
		// This option is basically a replacement for using "stb_image" library's
		// "stbi_set_flip_vertically_on_load(true)" method (in "Getting_Started" and "Lighting" projects).
		const aiScene* scene = modelImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			cout << "An error occured while loading the model!\n" << modelImporter.GetErrorString() << endl;

			return;
		}

		// Retrieve the path of directory containing the model.
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	// Each "Mesh" object contains a list of indices of meshes contained in the "Scene" object that belong to that
	// mesh. Function "processNode" is recursive, it keeps calling itself until all child nodes of "Scene" object
	// are processed, starting from the root node (the only child of "Scene" object).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Retrieve all meshes that belong to the current node, as indicated by the set of mesh indices stored in
		// the current node. Then process each mesh and add them to the global set of meshes in our "Model" class.
		for (unsigned int i = 0u; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// Recursively call "processNode" method for all children of current node.
		for (unsigned int i = 0u; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	// Translate an "aiMesh" object to our own "Mesh" object.
	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Translate each "aiVector3D" object to our own "Vertex" structure.
		vector<Vertex> vertices;
		for (unsigned int i = 0u; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 position = glm::vec3(0.0f);
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.position = position;

			glm::vec3 normal = glm::vec3(0.0f);
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.normal = normal;

			glm::vec2 texCoords = glm::vec2(0.0f);
			// "Assimp" library allows the model to have up to 8 different texture coordinates per vertex. We're
			// not going to use 8 of them, but only the first set of texture coordinates. Model may not have any
			// texture coordinates at all, so that's why an "if"-branch is needed.
			if (mesh->mTextureCoords[0u])
			{
				texCoords.x = mesh->mTextureCoords[0u][i].x;
				texCoords.y = mesh->mTextureCoords[0u][i].y;
				vertex.texCoords = texCoords;
			}
			else
			{
				vertex.texCoords = texCoords;
			}

			vertices.push_back(vertex);
		}

		// Translate each "aiFace" object to "unsigned int" variable.
		vector<unsigned int> indices;
		for (unsigned int i = 0u; i < mesh->mNumFaces; i++)
		{
			// "Assimp" library defines each mesh as having an array of faces, where each face represents a single
			// primitive. In our case, since we used the "aiProcess_Triangulate" post-processing option, all faces
			// are always triangles ("GL_TRIANGLES").
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0u; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		vector<Texture> textures;

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> loadTexturesOfMaterial(aiMaterial* material, aiTextureType type, string nameOfType);
public:
	Model(const char* path)
	{
		loadModelFrom(path);
	}

	void drawUsing(ShaderProgram& shaderProgram)
	{
		for (unsigned int i = 0u; i < meshes.size(); i++)
		{
			meshes[i].drawUsing(shaderProgram);
		}
	}
};
#endif
