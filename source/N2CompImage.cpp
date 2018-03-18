#include "ns/N2CompImage.h"
#include "ns/CompType.h"

#include <node2/CompImage.h>
#include <gum/ResPool.h>
#include <gum/Image.h>
#include <gum/Texture.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompImage::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompImage::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompImage::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompImage::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
}

void N2CompImage::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();
}

void N2CompImage::StoreToMem(const n0::SceneNodePtr& node, n2::CompImage& comp) const
{
	comp.SetFilepath(m_filepath);

	auto img = gum::ResPool::Instance().Fetch<gum::Image>(m_filepath);
	comp.SetTexture(img->GetTexture());
}

void N2CompImage::LoadFromMem(const n2::CompImage& comp)
{
	m_filepath = comp.GetFilepath();
}

}