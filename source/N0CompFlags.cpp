#include <ns/N0CompFlags.h>
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
	// tood
	return 0;
}

void N0CompFlags::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N0CompFlags::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N0CompFlags::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("visible", m_visible, alloc);
	val.AddMember("editable", m_editable, alloc);
}

void N0CompFlags::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_visible  = val["visible"].GetBool();
	m_editable = val["editable"].GetBool();
}

void N0CompFlags::StoreToMem(n0::CompFlags& comp) const
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