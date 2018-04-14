#include "ns/N3CompModel.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node3/CompModel.h>
#include <model/ModelFactory.h>
#include <model/Model.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N3CompModel::GetBinSize(const std::string& dir) const
{
	// todo
	return 0;
}

void N3CompModel::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N3CompModel::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N3CompModel::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
}

void N3CompModel::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();
}

void N3CompModel::StoreToMem(n3::CompModel& comp) const
{
	auto model = model::ModelFactory::Create(m_filepath);
	// todo
	// m_model->LoadFromJson(val);
	comp.SetModel(model);
}

void N3CompModel::LoadFromMem(const n3::CompModel& comp)
{
	m_filepath = comp.GetModel()->Type();
}

}