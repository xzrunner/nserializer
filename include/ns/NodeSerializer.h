#pragma once

#include <node0/typedef.h>

#include <rapidjson/document.h>

namespace bs { class ExportStream; class ImportStream; }
namespace n0 { class NodeSharedComp; }

namespace ns
{

class NodeSerializer
{
public:
	static bool StoreToJson(const n0::SceneNodePtr& node, const std::string& dir,
		rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	static bool LoadFromJson(n0::SceneNodePtr& node, const std::string& dir,
		const rapidjson::Value& val);

	static size_t GetBinSize(const n0::SceneNodePtr& node, const std::string& dir);
	static void StoreToBin(const n0::SceneNodePtr& node, const std::string& dir,
		bs::ExportStream& es);
	static void LoadFromBin(n0::SceneNodePtr& node, const std::string& dir,
		bs::ImportStream& is);

private:
	static bool StoreAssetCompToJson(const n0::NodeSharedComp& comp, const std::string& filepath);

}; // NodeSerializer

}