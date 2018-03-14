#pragma once

#include <node0/typedef.h>

#include <rapidjson/document.h>

namespace n0 { class NodeSharedComp; }

namespace ns
{

class NodeSerializer
{
public:
	static bool StoreNodeToJson(const n0::SceneNodePtr& node, const std::string& dir,
		rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	static bool LoadNodeFromJson(n0::SceneNodePtr& node, const std::string& dir,
		const rapidjson::Value& val);

	static bool StoreNodeAssetComp(const n0::NodeSharedComp& comp, const std::string& filepath);

}; // NodeSerializer

}