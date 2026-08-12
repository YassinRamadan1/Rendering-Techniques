#pragma once

#include<glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
private:
	unsigned int shader_program_id_ = 0;
public:

	Shader() = default;

	Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path);

	void createProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path);
	
	bool use();

	bool set3Float(const char* uniformName, float v1, float v2, float v3);

	bool set4Float(const char* uniformName, float v1, float v2, float v3, float v4);

	bool set1Float(const char* uniformName, float v);

	bool set2Float(const char* uniformName, float v1, float v2);

	bool setMatrix4f(const char* uniformName, bool Transpose, float* v);

	bool setMatrix3f(const char* UniformName, bool Transpose, float* v);

	bool set1Int(const char* uniformName, int v);
};