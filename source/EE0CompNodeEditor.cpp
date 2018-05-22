#include <ns/EE0CompNodeEditor.h>

#include <ee0/CompNodeEditor.h>

#include <boost/filesystem.hpp>

namespace ns
{

EE0CompNodeEditor::EE0CompNodeEditor()
	: m_visible(true)
	, m_editable(true)
{
}

size_t EE0CompNodeEditor::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void EE0CompNodeEditor::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void EE0CompNodeEditor::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void EE0CompNodeEditor::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("visible", m_visible, alloc);
	val.AddMember("editable", m_editable, alloc);
}

void EE0CompNodeEditor::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_visible  = val["visible"].GetBool();
	m_editable = val["editable"].GetBool();
}

void EE0CompNodeEditor::StoreToMem(ee0::CompNodeEditor& comp) const
{
	comp.SetVisible(m_visible);
	comp.SetEditable(m_editable);
}

void EE0CompNodeEditor::LoadFromMem(const ee0::CompNodeEditor& comp)
{
	m_visible  = comp.IsVisible();
	m_editable = comp.IsEditable();
}

}