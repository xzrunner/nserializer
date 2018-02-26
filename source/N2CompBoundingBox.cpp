#include "ns/N2CompBoundingBox.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node2/CompBoundingBox.h>

namespace ns
{

N2CompBoundingBox::N2CompBoundingBox()
	: m_xmin(0)
	, m_ymin(0)
	, m_xmax(0)
	, m_ymax(0)
{
}

size_t N2CompBoundingBox::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);   // type
	
	sz += sizeof(float) * 4; // size

	return sz;
}

void N2CompBoundingBox::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(COMP_BOUNDING_BOX)); // type

	// size
	es.Write(static_cast<float>(m_xmin));
	es.Write(static_cast<float>(m_ymin));
	es.Write(static_cast<float>(m_xmax));
	es.Write(static_cast<float>(m_ymax));
}

void N2CompBoundingBox::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	m_xmin = is.Float();
	m_ymin = is.Float();
	m_xmax = is.Float();
	m_ymax = is.Float();
}

void N2CompBoundingBox::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("xmin", m_xmin, alloc);
	val.AddMember("ymin", m_ymin, alloc);
	val.AddMember("xmax", m_xmax, alloc);
	val.AddMember("ymax", m_ymax, alloc);
}

void N2CompBoundingBox::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_xmin = val["xmin"].GetFloat();
	m_ymin = val["ymin"].GetFloat();
	m_xmax = val["xmax"].GetFloat();
	m_ymax = val["ymax"].GetFloat();
}

void N2CompBoundingBox::StoreToMem(n2::CompBoundingBox& comp) const
{
	sm::rect sz;
	sz.xmin = m_xmin;
	sz.ymin = m_ymin;
	sz.xmax = m_xmax;
	sz.ymax = m_ymax;
	comp.SetSize(sz);
}

void N2CompBoundingBox::LoadFromMem(const n2::CompBoundingBox& comp)
{
	auto& sz = comp.GetSize();
	m_xmin = sz.xmin;
	m_ymin = sz.ymin;
	m_xmax = sz.xmax;
	m_ymax = sz.ymax;
}

}