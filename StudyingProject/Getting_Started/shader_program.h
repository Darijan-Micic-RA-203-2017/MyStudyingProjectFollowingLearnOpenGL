// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the shader program header. This prevents linking conflicts.
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgram
{
private:
	// Utility function for checking whether the compilation of shader or linking of shader program succeeded.
	// Function prints out the error if it didn't.
	void checkForCompilationOrLinkingErrors(unsigned int shaderOrProgram, std::string type)
	{
		int success;
		char infoLog[512];
		if (type != "shader program")
		{
			glGetShaderiv(shaderOrProgram, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderOrProgram, 512, NULL, infoLog);
				std::cout << "Compilation of " << type << " has failed!\n" << infoLog << std::endl;

				if (type == "vertex shader")
				{
					errorCode = 4;
				}
				else
				{
					errorCode = 5;
				}

				return;
			}
		}
		else
		{
			glGetProgramiv(shaderOrProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderOrProgram, 512, NULL, infoLog);
				std::cout << "Linking of " << type << " has failed!\n" << infoLog << std::endl;

				errorCode = 6;

				return;
			}
		}

		errorCode = 0;
	}
public:
	unsigned int id = 0u;
	int errorCode = 0;

	ShaderProgram(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath)
	{
		// Retrieve shaders source code from files with paths provided as function parameters.
		std::string vsSourceCode;
		std::string fsSourceCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		// Ensure ifstream objects can throw exceptions.
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// Open files with source codes.
			vertexShaderFile.open(vertexShaderSourcePath);
			fragmentShaderFile.open(fragmentShaderSourcePath);

			std::stringstream vertexShaderStream;
			std::stringstream fragmentShaderStream;

			// Read file's buffer contents into streams.
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			// Close file handlers.
			fragmentShaderFile.close();
			vertexShaderFile.close();

			// Convert stream into string.
			vsSourceCode = vertexShaderStream.str();
			fsSourceCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "File with shader source code was not successfully read!\n" << e.what() << std::endl;

			return;
		}

		const char* vertexShaderSourceCode = vsSourceCode.c_str();
		const char* fragmentShaderSourceCode = fsSourceCode.c_str();

		// Create and compile shaders.
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
		glCompileShader(vertexShader);
		checkForCompilationOrLinkingErrors(vertexShader, "vertex shader");
		if (errorCode == 4)
		{
			glDeleteShader(vertexShader);

			return;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
		glCompileShader(fragmentShader);
		checkForCompilationOrLinkingErrors(fragmentShader, "fragment shader");
		if (errorCode == 5)
		{
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			return;
		}

		// Create and link shader program.
		id = glCreateProgram();
		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		glLinkProgram(id);
		checkForCompilationOrLinkingErrors(id, "shader program");
		if (errorCode == 6)
		{
			glDeleteProgram(id);
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}

	// Utility function for using (activating) shader program.
	void useProgram()
	{
		glUseProgram(id);
	}

	// Utility uniform functions.
	void setBoolUniform(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}

	void setUnsignedIntegerUniform(const std::string& name, unsigned int value) const
	{
		glUniform1ui(glGetUniformLocation(id, name.c_str()), value);
	}

	void setIntegerUniform(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void setFloatUniform(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

	void setFloatVec3Uniform(const std::string& name, float value0, float value1, float value2) const
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), value0, value1, value2);
	}

	void setFloatVec4Uniform(const std::string& name, float value0, float value1, float value2, float value3) const
	{
		glUniform4f(glGetUniformLocation(id, name.c_str()), value0, value1, value2, value3);
	}
};
#endif
