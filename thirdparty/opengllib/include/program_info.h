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
    std::uint32_t _id;
	int _numUniforms = 0;
	int _numAttributes = 0;

	std::map<std::string, int> _attribLocations;
	std::map<std::string, int> _uniformLocations;

public:
	program_info();
	void initialize(int programId);

	int getAttribLocation(const std::string &name) const;

	int getUniformLocation(const std::string &name) const;

	std::uint32_t id() { return _id; }
};

} // namespace ogllib

#endif