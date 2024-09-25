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

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
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
