#pragma once

#include <string>

namespace ns
{

enum ResFileType
{
	FILE_UNKNOWN = 0,
	FILE_IMAGE,
	FIME_JSON,
};

class ResFileHelper
{
public:
	static ResFileType Type(const std::string& filepath);

}; // ResFileHelper

}