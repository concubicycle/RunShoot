#ifndef __SHADER_PROGRAM_INFO__
#define __SHADER_PROGRAM_INFO__

#include "opengl_afx.h"
#include <vector>
#include <map>
#include <string>

namespace ogllib
{

class program_info
{
private:
	int _numUniforms;
	int _numAttributes;

	std::map<std::string, int> _attribLocations;
	std::map<std::string, int> _uniformLocations;

public:
	program_info();
	void initialize(int programId);

	int getAttribLocation(const std::string &name) const;

	int getUniformLocation(const std::string &name) const;
};

} // namespace ogllib

#endif