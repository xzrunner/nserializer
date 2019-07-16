#pragma once

#include <node0/typedef.h>

#include <rapidjson/document.h>

namespace n0 { class CompIdentity; }

namespace ns
{

class NodeFactory
{
public:
	static n0::SceneNodePtr Create(const std::string& dir, const rapidjson::Value& val);
	static n0::SceneNodePtr Create(const std::string& filepath);
	static n0::SceneNodePtr Create2D();
    static n0::SceneNodePtr Create3D();

private:
	static n0::SceneNodePtr CreateFromCommon(const std::string& filepath);
	static n0::SceneNodePtr CreateFromImage(const std::string& filepath);
	static n0::SceneNodePtr CreateFromImage3D(const std::string& filepath);
	static n0::SceneNodePtr CreateFromModel(const std::string& filepath);
	static n0::SceneNodePtr CreateFromScript(const std::string& filepath);

	static void InitCompId(n0::CompIdentity& cid, const std::string& filepath);

}; // NodeFactory

}