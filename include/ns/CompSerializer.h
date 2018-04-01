#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <rapidjson/document.h>

#include <functional>
#include <map>

namespace n0 { class NodeUniqueComp; class NodeSharedComp; }

namespace ns
{

class CompSerializer
{
public:
	using UniqueToJsonFunc = std::function<bool(const n0::NodeUniqueComp&, const std::string&,
		rapidjson::Value&, rapidjson::MemoryPoolAllocator<>&)>;
	using SharedToJsonFunc = std::function<bool(const n0::NodeSharedComp&, const std::string&,
		rapidjson::Value&, rapidjson::MemoryPoolAllocator<>&)>;
	using FromJsonFunc = std::function<void(n0::SceneNodePtr&, const std::string&, const rapidjson::Value&)>;
	using AssetFromJsonFunc = std::function<n0::CompAssetPtr(const std::string&, const rapidjson::Value&)>;

	void AddToJsonFunc(const std::string& name, const UniqueToJsonFunc& func);
	void AddToJsonFunc(const std::string& name, const SharedToJsonFunc& func);
	void AddFromJsonFunc(const std::string& name, const FromJsonFunc& func);
	void AddAssetFromJsonFunc(const std::string& name, const AssetFromJsonFunc& func);

	bool ToJson(const n0::NodeUniqueComp& comp, const std::string& dir, rapidjson::Value& val, 
		rapidjson::MemoryPoolAllocator<>& alloc) const;
	bool ToJson(const n0::NodeSharedComp& comp, const std::string& dir, rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const;
	void FromJson(n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val) const;
	n0::CompAssetPtr AssetFromJson(const std::string& dir, const rapidjson::Value& val) const;

private:
	std::map<std::string, UniqueToJsonFunc> m_unique_to_json;
	std::map<std::string, SharedToJsonFunc> m_shared_to_json;

	std::map<std::string, FromJsonFunc>      m_creator;
	std::map<std::string, AssetFromJsonFunc> m_asset_creator;

	CU_SINGLETON_DECLARATION(CompSerializer);

}; // CompSerializer

}