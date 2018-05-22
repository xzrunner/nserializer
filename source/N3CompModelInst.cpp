#include "ns/N3CompModelInst.h"
#include "ns/CompType.h"

#include <model/Model.h>
#include <node3/CompModelInst.h>

//#include <model/ModelInstance.h>

//#include <bs/ExportStream.h>
//#include <bs/ImportStream.h>
//#include <bs/FixedPointNum.h>
//#include <node3/CompModel.h>
//#include <model/Model.h>
#include <facade/ResPool.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N3CompModelInst::GetBinSize(const std::string& dir) const
{
	// todo
	return 0;
}

void N3CompModelInst::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N3CompModelInst::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N3CompModelInst::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	if (boost::filesystem::exists(m_filepath)) {
		std::string relative = boost::filesystem::relative(m_filepath, dir).string();
		val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
	} else {
		val.AddMember("filepath", rapidjson::Value(m_filepath.c_str(), alloc), alloc);
	}

	val.AddMember("anim", rapidjson::Value(m_anim_name.c_str(), alloc), alloc);
}

void N3CompModelInst::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();

	m_anim_name = val["anim"].GetString();
}

void N3CompModelInst::StoreToMem(n3::CompModelInst& comp) const
{
	auto model = facade::ResPool::Instance().Fetch<model::Model>(m_filepath);
	comp.SetModel(model, 0);
	comp.SetAnim(m_anim_name);
}

void N3CompModelInst::LoadFromMem(const n3::CompModelInst& comp)
{
	auto& model_inst = comp.GetModel();
	auto& model = model_inst->model;

	m_filepath = facade::ResPool::Instance().QueryFilepath<model::Model>(model);

	int idx = model_inst->curr_anim_index;
	auto& anims = model->anims;
	if (idx >= 0 && idx < static_cast<int>(anims.size())) {
		m_anim_name = anims[idx]->name;
	}
}

}