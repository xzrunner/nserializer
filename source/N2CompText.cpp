#include "ns/N2CompText.h"
#include "ns/CompType.h"

#include <node2/CompText.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompText::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompText::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompText::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompText::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value tb_val;
	StoreTBToJson(m_text.tb, tb_val, alloc);
	val.AddMember("textbox", tb_val, alloc);

	val.AddMember("text", rapidjson::Value(m_text.text.c_str(), alloc), alloc);
}

void N2CompText::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	LoadTBFromJson(m_text.tb, alloc, val["textbox"]);
	m_text.text = val["text"].GetString();
}

void N2CompText::StoreToMem(n2::CompText& comp) const
{
	comp.SetText(m_text);
}

void N2CompText::LoadFromMem(const n2::CompText& comp)
{
	m_text = comp.GetText();
}

void N2CompText::StoreTBToJson(const pt2::Textbox& tb, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
{
	val.SetObject();

	val.AddMember("width", tb.width, alloc);
	val.AddMember("height", tb.height, alloc);

	val.AddMember("font_type", tb.font_type, alloc);
	val.AddMember("font_size", tb.font_size, alloc);
	rapidjson::Value fcol_val;
	StoreColorToJson(tb.font_color, fcol_val, alloc);
	val.AddMember("font_color", fcol_val, alloc);

	val.AddMember("has_edge", tb.has_edge, alloc);
	val.AddMember("edge_size", tb.edge_size, alloc);
	rapidjson::Value ecol_val;
	StoreColorToJson(tb.edge_color, ecol_val, alloc);
	val.AddMember("edge_color", ecol_val, alloc);

	val.AddMember("align_hori", tb.align_hori, alloc);
	val.AddMember("align_vert", tb.align_vert, alloc);

	val.AddMember("space_hori", tb.space_hori, alloc);
	val.AddMember("space_vert", tb.space_vert, alloc);

	val.AddMember("overlabel", tb.overlabel, alloc);

	val.AddMember("richtext", tb.richtext, alloc);
}

void N2CompText::LoadTBFromJson(pt2::Textbox& tb, mm::LinearAllocator& alloc, const rapidjson::Value& val)
{
	tb.width  = val["width"].GetInt();
	tb.height = val["height"].GetInt();

	tb.font_type  = val["font_type"].GetInt();
	tb.font_size  = val["font_size"].GetInt();
	LoadColorFromJson(tb.font_color, alloc, val["font_color"]);

	tb.has_edge   = val["has_edge"].GetBool();
	tb.edge_size  = val["edge_size"].GetFloat();
	LoadColorFromJson(tb.edge_color, alloc, val["edge_color"]);

	tb.align_hori = static_cast<pt2::Textbox::HoriAlign>(val["align_hori"].GetInt());
	tb.align_vert = static_cast<pt2::Textbox::VertAlign>(val["align_vert"].GetInt());

	tb.space_hori = val["space_hori"].GetFloat();
	tb.space_vert = val["space_vert"].GetFloat();

	tb.overlabel = static_cast<pt2::Textbox::OverLabel>(val["overlabel"].GetInt());

	tb.richtext = val["richtext"].GetBool();
}

void N2CompText::StoreColorToJson(const pt2::GradientColor& col, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
{
	val.SetObject();
	val.AddMember("num", col.items.size(), alloc);
	val.AddMember("angle", col.angle, alloc);
	rapidjson::Value val_items;
	val_items.SetArray();
	for (auto& item : col.items) 
	{
		rapidjson::Value ival;
		ival.SetObject();
		ival.AddMember("col", item.col.ToRGBA(), alloc);
		ival.AddMember("pos", item.pos, alloc);
		val_items.PushBack(ival, alloc);
	}
	val.AddMember("items", val_items, alloc);
}

void N2CompText::LoadColorFromJson(pt2::GradientColor& col, mm::LinearAllocator& alloc, const rapidjson::Value& val)
{
	int num = val["num"].GetUint();
	col.items.clear();
	col.items.reserve(num);
	col.angle = val["angle"].GetFloat();
	for (auto& src : val["items"].GetArray())
	{
		pt2::GradientColor::Item dst;
		dst.col.FromRGBA(src["col"].GetUint());
		dst.pos = src["pos"].GetFloat();
		col.items.push_back(dst);
	}
}

}