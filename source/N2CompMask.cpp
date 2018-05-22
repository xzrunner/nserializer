#include "ns/N2CompMask.h"
#include "ns/CompType.h"
#include "ns/NodeFactory.h"

#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompMask.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompMask::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompMask::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompMask::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompMask::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string base_relative = boost::filesystem::relative(m_base_path, dir).string();
	val.AddMember("base_filepath", rapidjson::Value(base_relative.c_str(), alloc), alloc);

	std::string mask_relative = boost::filesystem::relative(m_mask_path, dir).string();
	val.AddMember("mask_filepath", rapidjson::Value(mask_relative.c_str(), alloc), alloc);
}

void N2CompMask::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto base_filepath = val["base_filepath"].GetString();
	m_base_path = boost::filesystem::absolute(base_filepath, dir).string();

	auto mask_filepath = val["mask_filepath"].GetString();
	m_mask_path = boost::filesystem::absolute(mask_filepath, dir).string();
}

void N2CompMask::StoreToMem(n2::CompMask& comp) const
{
	comp.SetBaseNode(NodeFactory::Create(m_base_path));
	comp.SetMaskNode(NodeFactory::Create(m_mask_path));
}

void N2CompMask::LoadFromMem(const n2::CompMask& comp)
{
	auto& base = comp.GetBaseNode();
	if (base) {
		auto& cid = base->GetUniqueComp<n0::CompIdentity>();
		m_base_path = cid.GetFilepath();
	}

	auto& mask = comp.GetMaskNode();
	if (mask) {
		auto& cid = mask->GetUniqueComp<n0::CompIdentity>();
		m_mask_path = cid.GetFilepath();
	}

}

}