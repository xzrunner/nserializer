#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <rapidjson/document.h>

#include <functional>
#include <map>
#include <array>

namespace n0 { class NodeComp; }
namespace bs { class ExportStream; class ImportStream; }

namespace ns
{

class CompSerializer
{
public:
	using ToJsonFunc = std::function<bool(const n0::NodeComp&, const std::string&,
		rapidjson::Value&, rapidjson::MemoryPoolAllocator<>&)>;
	using FromJsonFunc = std::function<void(n0::SceneNodePtr&, const std::string&, const rapidjson::Value&)>;
	using AssetFromJsonFunc = std::function<n0::CompAssetPtr(const std::string&, const rapidjson::Value&)>;

	void AddToJsonFunc(const std::string& name, const ToJsonFunc& func);
	void AddFromJsonFunc(const std::string& name, const FromJsonFunc& func);
	void AddAssetFromJsonFunc(const std::string& name, const AssetFromJsonFunc& func);

	bool ToJson(const n0::NodeComp& comp, const std::string& dir, rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const;
	void FromJson(n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val) const;
	n0::CompAssetPtr AssetFromJson(const std::string& dir, const rapidjson::Value& val) const;

	using GetBinSizeFunc = std::function<size_t(const n0::NodeComp&, const std::string&)>;
	using ToBinFunc = std::function<void(const n0::NodeComp&, const std::string&, bs::ExportStream&)>;
	using FromBinFunc = std::function<void(n0::SceneNodePtr&, const std::string&, bs::ImportStream& is)>;
	using AssetFromBinFunc = std::function<n0::CompAssetPtr(const std::string&, bs::ImportStream& is)>;

	void AddGetBinSizeFunc(const std::string& name, const GetBinSizeFunc& func);
	void AddToBinFunc(const std::string& name, const ToBinFunc& func);
	void AddFromBinFunc(const std::string& name, const FromBinFunc& func);
	void AddAssetFromBinFunc(const std::string& name, const AssetFromBinFunc& func);

	size_t GetBinSize(const n0::NodeComp& comp, const std::string& dir) const;
	void ToBin(const n0::NodeComp& comp, const std::string& dir, bs::ExportStream& es) const;
	void FromBin(n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is) const;
	n0::CompAssetPtr AssetFromBin(const std::string& dir, bs::ImportStream& is) const;

	void AddExtTypeToIndex(const std::string& type, uint32_t index) {
		m_ext_type_to_index.insert({type, index});
	}

public:
	enum CompIndex
	{
		COMP_INVALID = 0,

		COMP_N0_COMPLEX,
		COMP_N2_ANIM,
		COMP_N2_IMAGE,
		COMP_N2_MASK,
		COMP_N2_SCALE9,
		COMP_N2_TEXT,
		COMP_N3_MODEL,
		COMP_N3_MODEL_INST,

		COMP_N0_FLAGS,
		COMP_N0_ID,

		COMP_N2_COL_COMMON,
		COMP_N2_COL_MAP,

		COMP_N2_BOUNDING_BOX,
		COMP_N2_TRANSFORM,
		COMP_N3_AABB,
		COMP_N3_TRANSFORM,

		COMP_N2_SHARED_PATCH,
		COMP_N2_UNIQUE_PATCH,

		COMP_N2_SCISSOR,

		COMP_N2_SCRIPT,

		COMP_MAX,
	};

	CompIndex GetTypeIndex(const std::string& type) const;

private:
	std::map<std::string, ToJsonFunc>        m_to_json;
	std::map<std::string, FromJsonFunc>      m_from_json;
	std::map<std::string, AssetFromJsonFunc> m_asset_from_json;

	static const int MAX_CB_NUM = 64;
	std::array<GetBinSizeFunc, MAX_CB_NUM>   m_get_bin_sz;
	std::array<ToBinFunc, MAX_CB_NUM>        m_to_bin;
	std::array<FromBinFunc, MAX_CB_NUM>      m_from_bin;
	std::array<AssetFromBinFunc, MAX_CB_NUM> m_asset_from_bin;

	std::map<std::string, uint32_t> m_ext_type_to_index;

	CU_SINGLETON_DECLARATION(CompSerializer);

}; // CompSerializer

}