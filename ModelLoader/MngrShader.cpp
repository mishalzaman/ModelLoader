#include "MngrShader.h"

MngrShader::MngrShader()
{
	this->add("basic", "shaders/basic.vert", "shaders/basic.frag");
}

MngrShader::~MngrShader()
{
}

void MngrShader::add(std::string name, const char* vertexFilename, const char* fragmentFilename)
{
	unsigned int id = this->processShader(vertexFilename, fragmentFilename);
	if (id) this->shaders[name] = id;
}

void MngrShader::add(std::string name, const char* vertexFilename, const char* fragmentFilename, const char* geometryFilename)
{
	unsigned int id = this->processShader(vertexFilename, fragmentFilename, geometryFilename);
	if (id) this->shaders[name] = id;
}

void MngrShader::use(const std::string& name)
{
	if (!this->shaders[name]) {
		return;
	}

	glUseProgram(this->shaders[name]);
	this->activeShader = this->shaders[name];
}

void MngrShader::setBool(const std::string& property, bool value)
{
	if (!this->validateActiveShader()) return;
	glUniform1i(glGetUniformLocation(this->activeShader, property.c_str()), (int)value);
}

void MngrShader::setInt(const std::string& property, int value)
{
	glUniform1i(glGetUniformLocation(this->activeShader, property.c_str()), value);
}

void MngrShader::setFloat(const std::string& property, float value)
{
	glUniform1f(glGetUniformLocation(this->activeShader, property.c_str()), value);
}

void MngrShader::setVec2(const std::string& property, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(this->activeShader, property.c_str()), 1, &value[0]);
}
void MngrShader::setVec2(const std::string& property, float x, float y)
{
	glUniform2f(glGetUniformLocation(this->activeShader, property.c_str()), x, y);
}

void MngrShader::setVec3(const std::string& property, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(this->activeShader, property.c_str()), 1, &value[0]);
}
void MngrShader::setVec3(const std::string& property, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(this->activeShader, property.c_str()), x, y, z);
}

void MngrShader::setVec4(const std::string& property, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(this->activeShader, property.c_str()), 1, &value[0]);
}
void MngrShader::setVec4(const std::string& property, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(this->activeShader, property.c_str()), x, y, z, w);
}

void MngrShader::setMat2(const std::string& property, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(this->activeShader, property.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MngrShader::setMat3(const std::string& property, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(this->activeShader, property.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void MngrShader::setMat4(const std::string& property, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(this->activeShader, property.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int MngrShader::processShader(const char* vertexPath, const char* fragmentPath)
{
	unsigned int ID;
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// get the uniform block indices
	unsigned int uniformBlockIndexLights = glGetUniformBlockIndex(ID, "Lights");
	// link the shader's uniform block to the uniform binding point
	// Here we are binding the shader block index to buffer base 1
	glUniformBlockBinding(ID, uniformBlockIndexLights, 1);

	// get the uniform block indices
	unsigned int uniformBlockIndexMatrices = glGetUniformBlockIndex(ID, "Matrices");
	// link the shader's uniform block to the uniform binding point
	glUniformBlockBinding(ID, uniformBlockIndexMatrices, 0);

	return ID;
}

unsigned int MngrShader::processShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	unsigned int ID;

	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geometryPath);

		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment, geometry;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	checkCompileErrors(geometry, "GEOMETRY");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return ID;
}

void MngrShader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void MngrShader::resetActiveShader()
{
	this->activeShader = 0;
}

bool MngrShader::validateActiveShader()
{
	if (this->activeShader == 0) {
		return false;
	}

	return true;
}
