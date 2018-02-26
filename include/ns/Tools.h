#pragma once

#include <node0/typedef.h>

#include <rapidjson/document.h>

namespace ns
{

class Tools
{
public:
	static n0::SceneNodePtr CreateNode(const std::string& dir, const rapidjson::Value& val);
	static n0::SceneNodePtr CreateNode(const std::string& filepath);

}; // Tools

}