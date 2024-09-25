// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the model header. This prevents linking conflicts.
#ifndef MODEL_H
#define MODEL_H

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

	void loadModelFrom(const char* path);
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
