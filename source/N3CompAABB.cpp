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
	return sizeof(float) * 6;
}

void N3CompAABB::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	for (int i = 0; i < 3; ++i) {
		es.Write(static_cast<float>(m_aabb.Min()[i]));
	}
	for (int i = 0; i < 3; ++i) {
		es.Write(static_cast<float>(m_aabb.Max()[i]));
	}
}

void N3CompAABB::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	sm::vec3 min, max;
	for (int i = 0; i < 3; ++i) {
		min.xyz[i] = is.Float();
	}
	for (int i = 0; i < 3; ++i) {
		max.xyz[i] = is.Float();
	}

	m_aabb.Combine(min);
	m_aabb.Combine(max);
}

void N3CompAABB::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value val_min;
	val_min.SetArray();
	for (int i = 0; i < 3; ++i) {
		val_min.PushBack(m_aabb.Min()[i], alloc);
	}
	val.AddMember("min", val_min, alloc);

	rapidjson::Value val_max;
	val_max.SetArray();
	for (int i = 0; i < 3; ++i) {
		val_max.PushBack(m_aabb.Max()[i], alloc);
	}
	val.AddMember("max", val_max, alloc);
}

void N3CompAABB::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
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

void N3CompAABB::StoreToMem(const ur::Device& dev, n3::CompAABB& comp) const
{
	comp.SetAABB(m_aabb);
}

void N3CompAABB::LoadFromMem(const n3::CompAABB& comp)
{
	m_aabb = comp.GetAABB();
}

}