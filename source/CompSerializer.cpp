#include "ns/CompSerializer.h"
#include "ns/NodeFactory.h"

#include <guard/check.h>
#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/NodeUniqueComp.h>
#include <node0/NodeSharedComp.h>

#include <node0/CompComplex.h>
#include <node2/CompAnim.h>
#include <node2/CompImage.h>
#include <node2/CompMask.h>
#include <node2/CompScale9.h>
#include <node2/CompText.h>
#include <node3/CompModel.h>

#include <node0/CompFlags.h>
#include <node0/CompIdentity.h>
#include <node2/CompColorCommon.h>
#include <node2/CompColorMap.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompSharedPatch.h>
#include <node2/CompUniquePatch.h>
#include <node2/CompScissor.h>
#include <node2/CompScript.h>
#include <node3/CompAABB.h>
#include <node3/CompTransform.h>
#include <node3/CompModelInst.h>

#include <boost/filesystem.hpp>

namespace
{

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

CompIndex get_comp_idx(const std::string& name)
{
	if (name == n0::CompComplex::TYPE_NAME) {
		return COMP_N0_COMPLEX;
	} else if (name == n2::CompAnim::TYPE_NAME) {
		return COMP_N2_ANIM;
	} else if (name == n2::CompImage::TYPE_NAME) {
		return COMP_N2_IMAGE;
	} else if (name == n2::CompMask::TYPE_NAME) {
		return COMP_N2_MASK;
	} else if (name == n2::CompScale9::TYPE_NAME) {
		return COMP_N2_SCALE9;
	} else if (name == n2::CompText::TYPE_NAME) {
		return COMP_N2_TEXT;
	} else if (name == n3::CompModel::TYPE_NAME) {
		return COMP_N3_MODEL;
	} else if (name == n3::CompModelInst::TYPE_NAME) {
		return COMP_N3_MODEL_INST;
	} else if (name == n0::CompFlags::TYPE_NAME) {
		return COMP_N0_FLAGS;
	} else if (name == n0::CompIdentity::TYPE_NAME) {
		return COMP_N0_ID;
	} else if (name == n2::CompColorCommon::TYPE_NAME) {
		return COMP_N2_COL_COMMON;
	} else if (name == n2::CompColorMap::TYPE_NAME) {
		return COMP_N2_COL_MAP;
	} else if (name == n2::CompBoundingBox::TYPE_NAME) {
		return COMP_N2_BOUNDING_BOX;
	} else if (name == n2::CompTransform::TYPE_NAME) {
		return COMP_N2_TRANSFORM;
	} else if (name == n3::CompAABB::TYPE_NAME) {
		return COMP_N3_AABB;
	} else if (name == n3::CompTransform::TYPE_NAME) {
		return COMP_N3_TRANSFORM;
	} else if (name == n2::CompSharedPatch::TYPE_NAME) {
		return COMP_N2_SHARED_PATCH;
	} else if (name == n2::CompUniquePatch::TYPE_NAME) {
		return COMP_N2_UNIQUE_PATCH;
	} else if (name == n2::CompScissor::TYPE_NAME) {
		return COMP_N2_SCISSOR;
	} else if (name == n2::CompScript::TYPE_NAME) {
		return COMP_N2_SCRIPT;
	} else {
		GD_REPORT_ASSERT("unknown type.");
		return COMP_INVALID;
	}
}

}

namespace ns
{

CU_SINGLETON_DEFINITION(CompSerializer);

CompSerializer::CompSerializer()
{
}

void CompSerializer::AddToJsonFunc(const std::string& name, const UniqueToJsonFunc& func)
{
	auto status = m_unique_to_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

void CompSerializer::AddToJsonFunc(const std::string& name, const SharedToJsonFunc& func)
{
	auto status = m_shared_to_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

void CompSerializer::AddFromJsonFunc(const std::string& name, const FromJsonFunc& func)
{
	auto status = m_from_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

void CompSerializer::AddAssetFromJsonFunc(const std::string& name, const AssetFromJsonFunc& func)
{
	auto status = m_asset_from_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

bool CompSerializer::ToJson(const n0::NodeUniqueComp& comp,
	                        const std::string& dir,
	                        rapidjson::Value& val,
	                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
	auto itr = m_unique_to_json.find(comp.Type());
	if (itr != m_unique_to_json.end())
	{
		bool ret = itr->second(comp, dir, val, alloc);
		if (ret) {
			val.AddMember("comp_type", rapidjson::StringRef(comp.Type()), alloc);
		}
		return ret;
	}
	else
	{
		GD_REPORT_ASSERT("no comp creator");
		return false;
	}
}

bool CompSerializer::ToJson(const n0::NodeSharedComp& comp,
	                        const std::string& dir,
	                        rapidjson::Value& val,
	                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
	auto itr = m_shared_to_json.find(comp.Type());
	if (itr != m_shared_to_json.end())
	{
		val.SetObject();
		bool ret = itr->second(comp, dir, val, alloc);
		val.AddMember("comp_type", rapidjson::StringRef(comp.Type()), alloc);
		return ret;
	}
	else
	{
		GD_REPORT_ASSERT("no comp creator");
		return false;
	}
}

void CompSerializer::FromJson(n0::SceneNodePtr& node,
	                          const std::string& dir,
	                          const rapidjson::Value& val) const
{
	if (val.HasMember("comp_path"))
	{
		auto comp_path = val["comp_path"].GetString();
		auto absolute = boost::filesystem::absolute(comp_path, dir);
		NodeFactory::CreateNodeAssetComp(node, absolute.string());
		return;
	}

	auto type = val["comp_type"].GetString();
	auto itr = m_from_json.find(type);
	if (itr != m_from_json.end()) {
		itr->second(node, dir, val);
	} else {
		GD_REPORT_ASSERT("no comp creator");
	}
}

n0::CompAssetPtr CompSerializer::AssetFromJson(const std::string& dir, const rapidjson::Value& val) const
{
	if (val.HasMember("comp_path"))
	{
		auto comp_path = val["comp_path"].GetString();
		auto absolute = boost::filesystem::absolute(comp_path, dir);
		return NodeFactory::CreateAssetComp(absolute.string());
	}

	auto type = val["comp_type"].GetString();
	auto itr = m_asset_from_json.find(type);
	if (itr != m_asset_from_json.end()) {
		return itr->second(dir, val);
	} else {
		GD_REPORT_ASSERT("no comp creator");
		return nullptr;
	}
}

void CompSerializer::AddGetBinSizeFunc(const std::string& name, const UniqueGetBinSizeFunc& func)
{
	m_unique_get_bin_sz[get_comp_idx(name)] = func;
}

void CompSerializer::AddGetBinSizeFunc(const std::string& name, const SharedGetBinSizeFunc& func)
{
	m_shared_get_bin_sz[get_comp_idx(name)] = func;
}

void CompSerializer::AddToBinFunc(const std::string& name, const UniqueToBinFunc& func)
{
	m_unique_to_bin[get_comp_idx(name)] = func;
}

void CompSerializer::AddToBinFunc(const std::string& name, const SharedToBinFunc& func)
{
	m_shared_to_bin[get_comp_idx(name)] = func;
}

void CompSerializer::AddFromBinFunc(const std::string& name, const FromBinFunc& func)
{
	m_from_bin[get_comp_idx(name)] = func;
}

void CompSerializer::AddAssetFromBinFunc(const std::string& name, const AssetFromBinFunc& func)
{
	m_asset_from_bin[get_comp_idx(name)] = func;
}

size_t CompSerializer::GetBinSize(const n0::NodeUniqueComp& comp, const std::string& dir) const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);		// type
	uint8_t idx = get_comp_idx(comp.Type());
	sz += m_unique_get_bin_sz[idx](comp, dir);
	return sz;
}

size_t CompSerializer::GetBinSize(const n0::NodeSharedComp& comp, const std::string& dir) const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);		// type
	uint8_t idx = get_comp_idx(comp.Type());
	sz += m_shared_get_bin_sz[idx](comp, dir);
	return sz;
}

void CompSerializer::ToBin(const n0::NodeUniqueComp& comp, const std::string& dir, bs::ExportStream& es) const
{
	uint8_t idx = get_comp_idx(comp.Type());
	es.Write(idx);
	m_unique_to_bin[idx](comp, dir, es);
}

void CompSerializer::ToBin(const n0::NodeSharedComp& comp, const std::string& dir, bs::ExportStream& es) const
{
	uint8_t idx = get_comp_idx(comp.Type());
	es.Write(idx);
	m_shared_to_bin[idx](comp, dir, es);
}

void CompSerializer::FromBin(n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is) const
{
	//if (val.HasMember("comp_path"))
	//{
	//	auto comp_path = val["comp_path"].GetString();
	//	auto absolute = boost::filesystem::absolute(comp_path, dir);
	//	NodeFactory::CreateNodeAssetComp(node, absolute.string());
	//	return;
	//}

	CompIndex comp_idx = static_cast<CompIndex>(is.UInt8());
	m_from_bin[comp_idx](node, dir, is);
}

n0::CompAssetPtr CompSerializer::AssetFromBin(const std::string& dir, bs::ImportStream& is) const
{
	//if (val.HasMember("comp_path"))
	//{
	//	auto comp_path = val["comp_path"].GetString();
	//	auto absolute = boost::filesystem::absolute(comp_path, dir);
	//	return NodeFactory::CreateAssetComp(absolute.string());
	//}

	CompIndex comp_idx = static_cast<CompIndex>(is.UInt8());
	return m_asset_from_bin[comp_idx](dir, is);
}

}