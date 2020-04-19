#include "ns/N0CompFlags.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/CompFlags.h>
#include <node0/NodeFlags.h>

#include <boost/filesystem.hpp>

namespace ns
{

N0CompFlags::N0CompFlags()
	: m_visible(true)
	, m_editable(true)
{
}

size_t N0CompFlags::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;
	sz += sizeof(uint8_t);		// visible
	sz += sizeof(uint8_t);		// editable
	return sz;
}

void N0CompFlags::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(m_visible));
	es.Write(static_cast<uint8_t>(m_editable));
}

void N0CompFlags::LoadFromBin(const ur2::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	m_visible  = static_cast<bool>(is.UInt8());
	m_editable = static_cast<bool>(is.UInt8());
}

void N0CompFlags::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("visible", m_visible, alloc);
	val.AddMember("editable", m_editable, alloc);
}

void N0CompFlags::LoadFromJson(const ur2::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_visible  = val["visible"].GetBool();
	m_editable = val["editable"].GetBool();
}

void N0CompFlags::StoreToMem(const ur2::Device& dev, n0::CompFlags& comp) const
{
	comp.SetFlag<n0::NodeNotVisible>(!m_visible);
	comp.SetFlag<n0::NodeNotEditable>(!m_editable);
}

void N0CompFlags::LoadFromMem(const n0::CompFlags& comp)
{
	m_visible = !comp.GetFlag<n0::NodeNotVisible>();
	m_editable = !comp.GetFlag<n0::NodeNotEditable>();
}

}