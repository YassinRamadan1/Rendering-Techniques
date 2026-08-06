#include "shader.h"

Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path)
{
	createProgram(vertex_shader_path, fragment_shader_path, geometry_shader_path);
}

void Shader::createProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path)
{
	int success;
	char information_log[1000];
	std::ifstream vertex_shader_file, fragment_shader_file;
	std::stringstream vertex_shader_stream, fragment_shader_stream;
	std::string vertex_shader_string, fragment_shader_string;
	const char* vertex_shader_code, * fragment_shader_code;

	vertex_shader_file.open(vertex_shader_path);
	fragment_shader_file.open(fragment_shader_path);

	if (!vertex_shader_file.is_open()) {
		std::cout << "Failed to open vertex shader file: " << vertex_shader_path << std::endl;
		return;
	}

	if (!fragment_shader_file.is_open()) {
		std::cout << "Failed to open fragment shader file: " << fragment_shader_path << std::endl;
		return;
	}

	vertex_shader_stream << vertex_shader_file.rdbuf();
	fragment_shader_stream << fragment_shader_file.rdbuf();

	vertex_shader_file.close();
	fragment_shader_file.close();

	vertex_shader_string = vertex_shader_stream.str();
	fragment_shader_string = fragment_shader_stream.str();
	vertex_shader_code = vertex_shader_string.c_str();
	fragment_shader_code = fragment_shader_string.c_str();

	unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_shader_code, NULL);
	glCompileShader(vertex_shader_id);

	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader_id, 1000, NULL, information_log);
		std::cout << "Vertex Shader Compiling Error!\n" << information_log << '\n';
	}

	unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment_shader_id);

	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader_id, 1000, NULL, information_log);
		std::cout << "Fragment Shader Compiling Error!\n" << information_log << '\n';
	}

	shader_program_id_ = glCreateProgram();
	glAttachShader(shader_program_id_, vertex_shader_id);
	glAttachShader(shader_program_id_, fragment_shader_id);

	unsigned int geometry_shader_id;
	if (!geometry_shader_path.empty())
	{
		std::ifstream geometry_shader_file;
		std::stringstream geometry_shader_stream;
		std::string geometry_shader_string;
		const char* geometry_shader_code;
		
		geometry_shader_file.open(geometry_shader_path);

		if (!geometry_shader_file.is_open()) {
			std::cout << "Failed to open geometry shader file: " << vertex_shader_path << std::endl;
			return;
		}

		geometry_shader_stream << geometry_shader_file.rdbuf();

		geometry_shader_file.close();

		geometry_shader_string = geometry_shader_stream.str();
		geometry_shader_code = geometry_shader_string.c_str();

		geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader_id, 1, &geometry_shader_code, NULL);
		glCompileShader(geometry_shader_id);

		glGetShaderiv(geometry_shader_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry_shader_id, 1000, NULL, information_log);
			std::cout << "Geometry Shader Compiling Error!\n" << information_log << '\n';
		}

		glAttachShader(shader_program_id_, geometry_shader_id);
	}

	glLinkProgram(shader_program_id_);

	glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program_id_, 1000, NULL, information_log);
		std::cout << "Shader Program Linking Error!\n" << information_log << '\n';
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	if (!geometry_shader_path.empty())
	{
		glDeleteShader(geometry_shader_id);
	}
}

bool Shader::use()
{
	if (shader_program_id_ == 0)
		return false;
	glUseProgram(shader_program_id_);
	return true;
}

bool Shader::set3Float(const char* uniformName, float v1, float v2, float v3)
{
	int location = glGetUniformLocation(shader_program_id_, uniformName);
	if (location == -1)
		return false;
	glUniform3f(location, v1, v2, v3);
}

bool Shader::set4Float(const char* uniformName, float v1, float v2, float v3, float v4)
{
	int location = glGetUniformLocation(shader_program_id_, uniformName);
	if (location == -1)
		return false;
	glUniform4f(location, v1, v2, v3, v4);
	return true;
}

bool Shader::set1Float(const char* uniformName, float v)
{
	int location = glGetUniformLocation(shader_program_id_, uniformName);
	if (location == -1)
		return false;
	glUniform1f(location, v);
	return true;
}

bool Shader::set2Float(const char* uniformName, float v1, float v2)
{
	int location = glGetUniformLocation(shader_program_id_, uniformName);
	if (location == -1)
		return false;
	glUniform2f(location, v1, v2);
	return true;
}

bool Shader::setMatrix4f(const char* uniformName, bool Transpose, float* v)
{
	int location = glGetUniformLocation(shader_program_id_, uniformName);
	if (location == -1)
		return false;
	glUniformMatrix4fv(location, 1, Transpose, v);
	return true;
}

bool Shader::setMatrix3f(const char* UniformName, bool Transpose, float* v)
{
	int location = glGetUniformLocation(shader_program_id_, UniformName);
	if (location == -1)
		return false;
	glUniformMatrix3fv(location, 1, Transpose, v);
	return true;
}

bool Shader::set1Int(const char* uniformName, int v)
{
	int location = glGetUniformLocation(shader_program_id_, uniformName);
	if (location == -1)
		return false;
	glUniform1i(location, v);
	return true;
}