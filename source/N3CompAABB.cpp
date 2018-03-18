#include "ns/N3CompAABB.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node3/CompAABB.h>

namespace ns
{

size_t N3CompAABB::GetBinSize(const std::string& dir) const
{
	// todo
	return 0;
}

void N3CompAABB::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N3CompAABB::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N3CompAABB::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();
	
	rapidjson::Value val_min;
	auto& min = m_aabb.Min();
	val_min.SetArray();
	val_min.PushBack(min.x, alloc);
	val_min.PushBack(min.y, alloc);
	val_min.PushBack(min.z, alloc);
	val.AddMember("min", val_min, alloc);
	
	rapidjson::Value val_max;
	auto& max = m_aabb.Max();
	val_max.SetArray();
	val_max.PushBack(max.x, alloc);
	val_max.PushBack(max.y, alloc);
	val_max.PushBack(max.z, alloc);
	val.AddMember("max", val_max, alloc);
}

void N3CompAABB::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	sm::vec3 min, max;
	min.x = val["min"][0].GetFloat();
	min.y = val["min"][1].GetFloat();
	min.z = val["min"][2].GetFloat();
	max.x = val["max"][0].GetFloat();
	max.y = val["max"][1].GetFloat();
	max.z = val["max"][2].GetFloat();
	m_aabb.Combine(min);
	m_aabb.Combine(max);
}

void N3CompAABB::StoreToMem(const n0::SceneNodePtr& node, n3::CompAABB& comp) const
{
	comp.SetAABB(m_aabb);
}

void N3CompAABB::LoadFromMem(const n3::CompAABB& comp)
{
	m_aabb = comp.GetAABB();
}

}