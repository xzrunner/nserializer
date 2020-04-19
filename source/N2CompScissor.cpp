#include "ns/N2CompScissor.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node2/CompScissor.h>

namespace ns
{

size_t N2CompScissor::GetBinSize(const std::string& dir) const
{
	return sizeof(float) * 4;
}

void N2CompScissor::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<float>(m_rect.xmin));
	es.Write(static_cast<float>(m_rect.ymin));
	es.Write(static_cast<float>(m_rect.xmax));
	es.Write(static_cast<float>(m_rect.ymax));
}

void N2CompScissor::LoadFromBin(const ur2::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	m_rect.xmin = is.Float();
	m_rect.ymin = is.Float();
	m_rect.xmax = is.Float();
	m_rect.ymax = is.Float();
}

void N2CompScissor::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("xmin", m_rect.xmin, alloc);
	val.AddMember("ymin", m_rect.ymin, alloc);
	val.AddMember("xmax", m_rect.xmax, alloc);
	val.AddMember("ymax", m_rect.ymax, alloc);
}

void N2CompScissor::LoadFromJson(const ur2::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_rect.xmin = val["xmin"].GetFloat();
	m_rect.ymin = val["ymin"].GetFloat();
	m_rect.xmax = val["xmax"].GetFloat();
	m_rect.ymax = val["ymax"].GetFloat();
}

void N2CompScissor::StoreToMem(const ur2::Device& dev, n2::CompScissor& comp) const
{
	comp.SetRect(m_rect);
}

void N2CompScissor::LoadFromMem(const n2::CompScissor& comp)
{
	m_rect = comp.GetRect();
}

}