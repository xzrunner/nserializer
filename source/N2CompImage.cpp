#include "ns/N2CompImage.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node2/CompImage.h>
#include <facade/ResPool.h>
#include <facade/Image.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompImage::GetBinSize(const std::string& dir) const
{
	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	return bs::pack_size(relative);
}

void N2CompImage::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	es.Write(relative);
}

void N2CompImage::LoadFromBin(const std::string& dir, bs::ImportStream& is)
{
	auto relative = is.String();
	m_filepath = boost::filesystem::absolute(relative, dir).string();
}

void N2CompImage::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
}

void N2CompImage::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto relative = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(relative, dir).string();
}

void N2CompImage::StoreToMem(n2::CompImage& comp) const
{
	comp.SetFilepath(m_filepath);

	auto img = facade::ResPool::Instance().Fetch<facade::Image>(m_filepath);
	comp.SetTexture(img->GetTexture());
}

void N2CompImage::LoadFromMem(const n2::CompImage& comp)
{
	m_filepath = comp.GetFilepath();
}

}