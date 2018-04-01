#include "ns/N2CompScissor.h"

#include <node2/CompScissor.h>

namespace ns
{

size_t N2CompScissor::GetBinSize(const std::string& dir) const
{
	// todo
	size_t sz = 0;
	return sz;
}

void N2CompScissor::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompScissor::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompScissor::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("xmin", m_rect.xmin, alloc);
	val.AddMember("ymin", m_rect.ymin, alloc);
	val.AddMember("xmax", m_rect.xmax, alloc);
	val.AddMember("ymax", m_rect.ymax, alloc);
}

void N2CompScissor::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_rect.xmin = val["xmin"].GetFloat();
	m_rect.ymin = val["ymin"].GetFloat();
	m_rect.xmax = val["xmax"].GetFloat();
	m_rect.ymax = val["ymax"].GetFloat();
}

void N2CompScissor::StoreToMem(n2::CompScissor& comp) const
{
	comp.SetRect(m_rect);
}

void N2CompScissor::LoadFromMem(const n2::CompScissor& comp)
{
	m_rect = comp.GetRect();
}

}