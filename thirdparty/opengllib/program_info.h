#ifndef __SHADER_PROGRAM_INFO__
#define __SHADER_PROGRAM_INFO__

#include "opengl_afx.h"
#include <vector>
#include <map>
#include <string>

class ProgramInfo
{
private: 
	int _numUniforms;
	int _numAttributes;

	std::map<std::string, int> _attribLocations;
	std::map<std::string, int> _uniformLocations;

public:
	ProgramInfo();
	void initialize(int programId);

	int getAttribLocation(const std::string& name) const;

	int getUniformLocation(const std::string& name) const;
};

#endif