// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the model header. This prevents linking conflicts.
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
using namespace std;
#include "shader_program.h"
#include "stb_image.h"
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

		// Translate "aiMaterial" object to a vector of our own "Texture" structures.
		vector<Texture> textures;
		unsigned int indexOfMeshMaterial = mesh->mMaterialIndex;
		// Model may not have a material (consisting of textures) at all, so that's why an "if"-branch is needed.
		if (indexOfMeshMaterial)
		{
			aiMaterial* meshMaterial = scene->mMaterials[indexOfMeshMaterial];

			vector<Texture> diffuseMaps = 
				loadTexturesOfMaterial(meshMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
			// Insert all diffuse maps (from the beginning to the end of the returned vector) to the end of the
			// textures vector.
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specularMaps = 
				loadTexturesOfMaterial(meshMaterial, aiTextureType_SPECULAR, "texture_specular");
			// Insert all specular maps (from the beginning to the end of the returned vector) to the end of the
			// textures vector.
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			vector<Texture> emissionMaps = 
				loadTexturesOfMaterial(meshMaterial, aiTextureType_EMISSIVE, "texture_emission");
			// Insert all emission maps (from the beginning to the end of the returned vector) to the end of the
			// textures vector.
			textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	// Retrieve, load and initialize the textures of the material. Function returns a vector of textures that we
	// insert at the end of the model's textures vector.
	// NOTE: We make the assumption that texture file paths in model files are local to the actual model object,
	// e.g. in the same directory as the location of the model itself. We can then simply concatenate the texture
	// location string and the directory string we retrieved earlier (in the "loadModelFrom" function) to get the
	// complete texture path (that's why the "loadTextureFromFile" function also needs the directory string).
	// Some models found on the internet use absolute paths for their texture locations, which won't work on each
	// machine. In that case you probably want to manually edit the file to use local paths for the textures
	// (if possible). 
	vector<Texture> loadTexturesOfMaterial(aiMaterial* material, aiTextureType type, string nameOfType)
	{
		// Iterate over all the texture locations of the given type of texture.
		vector<Texture> textures;
		for (unsigned int i = 0u; i < material->GetTextureCount(type); i++)
		{
			// Retrieve the location of the texture file.
			aiString str;
			material->GetTexture(type, i, &str);

			// Load the texture from file.
			Texture texture;
			texture.id = loadTextureFromFile(str.C_Str(), directory);
			texture.type = nameOfType;
			texture.path = str.C_Str();

			textures.push_back(texture);
		}

		return textures;
	}

	// Load the texture image file, create the texture, set texture wrapping and filtering parameters and finally
	// generate the texture and all its required mipmaps.
	unsigned int loadTextureFromFile(const char* path, const string& directory)
	{
		// Create memory on the GPU where texture will be stored.
		unsigned int texture;
		glGenTextures(1, &texture);
		// Bind (assign) the newly created texture to OpenGL's context.
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set texture wrapping parameters. Texture coordinates are in range [0.0f, 1.0f]. If texture coordinates
		// are specified outside of mentioned range, texture wrapping option determines the look.
		// Each texture wrapping option can be set per coordinate axis (s, t and r if 3D textures are used).
		// s-axis, t-axis and r-axis correspond to x-axis, y-axis and z-axis, respectively.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture filtering parameters. Texture coordinates do not depend on resolution, but can be any
		// floating point value. Therefore, OpenGL needs to figure out which texture pixel (texel) to map the
		// texture coordinate to. Nearest neighbour filtering is better suited for minifying operations, while
		// (bi)linear filtering is better suited for magnifying operations.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Mipmaps are used to improve minifying, not magnifying. Setting one of the mipmap filtering options as
		// the magnification filter will generate the OpenGL "GL_INVALID_ENUM" error code.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Form the filename by concatenation.
		string filename = string(path);
		filename = directory + "/" + filename;
		// Load the image that will be used as a texture.
		int textureImageWidth, textureImageHeight, numberOfColorChannelsInTextureImage;
		unsigned char* pixels = stbi_load(filename.c_str(), &textureImageWidth, &textureImageHeight, 
			&numberOfColorChannelsInTextureImage, 0);
		if (pixels)
		{
			// Determine image format from number of color channels in texture image.
			GLenum format = 0u;
			switch (numberOfColorChannelsInTextureImage)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				stbi_image_free(pixels);
				glBindTexture(GL_TEXTURE_2D, 0u);
				glDeleteTextures(1, &texture);

				return;
			}

			// Generate a texture using the previously loaded image data (pixels).
			glTexImage2D(GL_TEXTURE_2D, 0, format, textureImageWidth, textureImageHeight, 0, format, 
				GL_UNSIGNED_BYTE, pixels);
			// Automatically generate all the required mipmaps for the currently bound texture.
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Image of would-be-texture could not be loaded!" << std::endl;
			glBindTexture(GL_TEXTURE_2D, 0u);
			glDeleteTextures(1, &texture);
		}
		// Free the image memory.
		stbi_image_free(pixels);

		return texture;
	}
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
