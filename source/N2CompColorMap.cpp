#include "ns/N2CompColorMap.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node2/CompColorMap.h>

namespace ns
{

N2CompColorMap::N2CompColorMap()
	: m_rmap(0)
	, m_gmap(0)
	, m_bmap(0)
{
}

size_t N2CompColorMap::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);  // type

	sz += sizeof(uint32_t);	// rmap
	sz += sizeof(uint32_t);	// gmap
	sz += sizeof(uint32_t);	// bmap

	return sz;
}

void N2CompColorMap::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(COMP_COLOR_MAP)); // type

	es.Write(static_cast<uint32_t>(m_rmap));        // rmap
	es.Write(static_cast<uint32_t>(m_gmap));        // gmap
	es.Write(static_cast<uint32_t>(m_bmap));        // bmap
}

void N2CompColorMap::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	m_rmap = is.UInt32();
	m_gmap = is.UInt32();
	m_bmap = is.UInt32();
}

void N2CompColorMap::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("rmap", m_rmap, alloc);
	val.AddMember("gmap", m_gmap, alloc);
	val.AddMember("bmap", m_bmap, alloc);
}

void N2CompColorMap::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_rmap = val["rmap"].GetUint();
	m_gmap = val["gmap"].GetUint();
	m_bmap = val["bmap"].GetUint();
}

void N2CompColorMap::StoreToMem(const ur::Device& dev, n2::CompColorMap& comp) const
{
	pt2::RenderColorMap col;
	col.rmap.FromRGBA(m_rmap);
	col.rmap.FromRGBA(m_rmap);
	col.rmap.FromRGBA(m_rmap);

	comp.SetColor(col);
}

void N2CompColorMap::LoadFromMem(const n2::CompColorMap& comp)
{
	auto& col = comp.GetColor();
	m_rmap = col.rmap.ToRGBA();
	m_gmap = col.gmap.ToRGBA();
	m_bmap = col.bmap.ToRGBA();
}

}