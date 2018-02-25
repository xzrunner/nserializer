#include <ns/CompNodeEditor.h>

#include <ee0/CompNodeEditor.h>

#include <boost/filesystem.hpp>

namespace ns
{

CompNodeEditor::CompNodeEditor()
	: m_visible(true)
	, m_editable(true)
{
}

size_t CompNodeEditor::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void CompNodeEditor::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void CompNodeEditor::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void CompNodeEditor::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);

	val.AddMember("name", rapidjson::Value(m_name.c_str(), alloc), alloc);

	val.AddMember("visible", m_visible, alloc);
	val.AddMember("editable", m_editable, alloc);
}

void CompNodeEditor::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();

	m_name = val["name"].GetString();

	m_visible  = val["visible"].GetBool();
	m_editable = val["editable"].GetBool();
}

void CompNodeEditor::StoreToMem(ee0::CompNodeEditor& comp) const
{
	comp.SetFilepath(m_filepath);
	comp.SetName(m_name);
	comp.SetVisible(m_visible);
	comp.SetEditable(m_editable);
}

void CompNodeEditor::LoadFromMem(const ee0::CompNodeEditor& comp)
{
	m_filepath = comp.GetFilepath();
	m_name     = comp.GetName();
	m_visible  = comp.IsVisible();
	m_editable = comp.IsEditable();
}

}