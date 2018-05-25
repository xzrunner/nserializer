#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <rapidjson/document.h>

#include <functional>
#include <map>
#include <array>

namespace n0 { class NodeUniqueComp; class NodeSharedComp; }
namespace bs { class ExportStream; class ImportStream; }

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

	using UniqueGetBinSizeFunc = std::function<size_t(const n0::NodeUniqueComp&, const std::string&)>;
	using SharedGetBinSizeFunc = std::function<size_t(const n0::NodeSharedComp&, const std::string&)>;
	using UniqueToBinFunc = std::function<void(const n0::NodeUniqueComp&, const std::string&, bs::ExportStream&)>;
	using SharedToBinFunc = std::function<void(const n0::NodeSharedComp&, const std::string&, bs::ExportStream&)>;
	using FromBinFunc = std::function<void(n0::SceneNodePtr&, const std::string&, bs::ImportStream& is)>;
	using AssetFromBinFunc = std::function<n0::CompAssetPtr(const std::string&, bs::ImportStream& is)>;

	void AddGetBinSizeFunc(const std::string& name, const UniqueGetBinSizeFunc& func);
	void AddGetBinSizeFunc(const std::string& name, const SharedGetBinSizeFunc& func);
	void AddToBinFunc(const std::string& name, const UniqueToBinFunc& func);
	void AddToBinFunc(const std::string& name, const SharedToBinFunc& func);
	void AddFromBinFunc(const std::string& name, const FromBinFunc& func);
	void AddAssetFromBinFunc(const std::string& name, const AssetFromBinFunc& func);

	size_t GetBinSize(const n0::NodeUniqueComp& comp, const std::string& dir) const;
	size_t GetBinSize(const n0::NodeSharedComp& comp, const std::string& dir) const;
	void ToBin(const n0::NodeUniqueComp& comp, const std::string& dir, bs::ExportStream& es) const;
	void ToBin(const n0::NodeSharedComp& comp, const std::string& dir, bs::ExportStream& es) const;
	void FromBin(n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is) const;
	n0::CompAssetPtr AssetFromBin(const std::string& dir, bs::ImportStream& is) const;

private:
	std::map<std::string, UniqueToJsonFunc>  m_unique_to_json;
	std::map<std::string, SharedToJsonFunc>  m_shared_to_json;
	std::map<std::string, FromJsonFunc>      m_from_json;
	std::map<std::string, AssetFromJsonFunc> m_asset_from_json;

	static const int MAX_CB_NUM = 64;
	std::array<UniqueGetBinSizeFunc, MAX_CB_NUM> m_unique_get_bin_sz;
	std::array<SharedGetBinSizeFunc, MAX_CB_NUM> m_shared_get_bin_sz;
	std::array<UniqueToBinFunc, MAX_CB_NUM>      m_unique_to_bin;
	std::array<SharedToBinFunc, MAX_CB_NUM>      m_shared_to_bin;
	std::array<FromBinFunc, MAX_CB_NUM>          m_from_bin;
	std::array<AssetFromBinFunc, MAX_CB_NUM>     m_asset_from_bin;

	CU_SINGLETON_DECLARATION(CompSerializer);

}; // CompSerializer

}