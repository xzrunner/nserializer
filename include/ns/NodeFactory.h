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

	static void CreateNodeAssetComp(n0::SceneNodePtr& node, const std::string& filepath);
	static n0::CompAssetPtr CreateAssetComp(const std::string& filepath);

private:
	static n0::SceneNodePtr CreateFromImage(const std::string& filepath);
	static n0::SceneNodePtr CreateFromJson(const std::string& filepath);
	static n0::SceneNodePtr CreateFromModel(const std::string& filepath);

	static void InitCompId(n0::CompIdentity& cid, const std::string& filepath);

}; // NodeFactory

}