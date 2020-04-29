#include "ns/N3CompModel.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node3/CompModel.h>
#include <model/Model.h>
#include <facade/ResPool.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N3CompModel::GetBinSize(const std::string& dir) const
{
	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	return bs::pack_size(relative);
}

void N3CompModel::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	es.Write(relative);
}

void N3CompModel::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	auto relative = is.String();
	m_filepath = boost::filesystem::absolute(relative, dir).string();
}

void N3CompModel::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	if (boost::filesystem::exists(m_filepath)) {
		std::string relative = boost::filesystem::relative(m_filepath, dir).string();
		val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
	} else {
		val.AddMember("filepath", rapidjson::Value(m_filepath.c_str(), alloc), alloc);
	}
}

void N3CompModel::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();
}

void N3CompModel::StoreToMem(const ur::Device& dev, n3::CompModel& comp) const
{
	auto model = facade::ResPool::Instance().Fetch<model::Model>(m_filepath, &dev);
	comp.SetModel(model);
	comp.SetFilepath(m_filepath);
}

void N3CompModel::LoadFromMem(const n3::CompModel& comp)
{
	m_filepath = comp.GetFilepath();
}

}