#ifndef MNGR_SHADER_H
#define MNGR_SHADER_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

class MngrShader {
	public:
		MngrShader();
		~MngrShader();
		void add(std::string name, const char* vertexFilename, const char* fragmentFilename);
		void add(std::string name, const char* vertexFilename, const char* fragmentFilename, const char* geometryFilename);
		void use(const std::string& name);
		void setBool(const std::string& property, bool value);
		void setInt(const std::string& property, int value);
		void setFloat(const std::string& property, float value);
		void setVec2(const std::string& property, const glm::vec2& value);
		void setVec2(const std::string& property, float x, float y);
		void setVec3(const std::string& property, const glm::vec3& value);
		void setVec3(const std::string& property, float x, float y, float z);
		void setVec4(const std::string& property, const glm::vec4& value);
		void setVec4(const std::string& property, float x, float y, float z, float w);
		void setMat2(const std::string& property, const glm::mat2& mat);
		void setMat3(const std::string& property, const glm::mat3& mat);
		void setMat4(const std::string& property, const glm::mat4& mat);
	private:
		unsigned int processShader(const char* vertexPath, const char* fragmentPath);
		unsigned int processShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
		void checkCompileErrors(unsigned int shader, std::string type);
		std::unordered_map<std::string, unsigned int> shaders;
		unsigned int activeShader = 0;
		void resetActiveShader();
		bool validateActiveShader();
};

#endif