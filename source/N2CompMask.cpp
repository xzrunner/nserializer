#include "ns/N2CompMask.h"
#include "ns/CompType.h"
#include "ns/Tools.h"

#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>
#include <node2/CompMask.h>
#include <gum/ResPool.h>
#include <gum/Image.h>
#include <gum/Texture.h>

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
	comp.SetBaseNode(Tools::CreateNode(m_base_path));
	comp.SetMaskNode(Tools::CreateNode(m_mask_path));
}

void N2CompMask::LoadFromMem(const n2::CompMask& comp)
{
	auto& base = comp.GetBaseNode();
	if (base) {
		auto& base_editor = base->GetComponent<ee0::CompNodeEditor>();
		m_base_path = base_editor.GetFilepath();
	}

	auto& mask = comp.GetMaskNode();
	if (mask) {
		auto& mask_editor = mask->GetComponent<ee0::CompNodeEditor>();
		m_mask_path = mask_editor.GetFilepath();
	}

}

}