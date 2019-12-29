#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "MngrShader.h"
#include "OGLGenTexture.h"
#include "Shader.h"

class ModelLoader
{
	public:
		ModelLoader();
		~ModelLoader();
		void loadModel(std::string filename);
		void draw(glm::mat4 &projection, glm::mat4 &view, Shader &shader);
	private:
		glm::mat4 model;
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> texturesLoaded;
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Vertex> vertices(aiMesh* mesh);
		std::vector<unsigned int> indices(aiMesh* mesh);
		std::vector<Texture> textures(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif

