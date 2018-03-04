#pragma once

#include <node0/typedef.h>

#include <rapidjson/document.h>

namespace ns
{

class NodeFactory
{
public:
	static n0::SceneNodePtr Create(const std::string& dir, const rapidjson::Value& val);
	static n0::SceneNodePtr Create(const std::string& filepath);

private:
	static n0::SceneNodePtr CreateFromImage(const std::string& filepath);
	static n0::SceneNodePtr CreateFromJson(const std::string& filepath);

}; // NodeFactory

}