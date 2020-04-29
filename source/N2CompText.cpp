#include "ns/N2CompText.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node2/CompText.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompText::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;
	sz += GetTextboxBinSize(m_text.tb);
	sz += bs::pack_size(m_text.text);
	return sz;
}

void N2CompText::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	StoreTextboxToBin(m_text.tb, es);
	es.Write(m_text.text);
}

void N2CompText::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	LoadTextboxFromBin(m_text.tb, is);
	m_text.text = is.String();
}

void N2CompText::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value tb_val;
	StoreTextboxToJson(m_text.tb, tb_val, alloc);
	val.AddMember("textbox", tb_val, alloc);

	val.AddMember("text", rapidjson::Value(m_text.text.c_str(), alloc), alloc);
}

void N2CompText::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	LoadTextboxFromJson(m_text.tb, alloc, val["textbox"]);
	m_text.text = val["text"].GetString();
}

void N2CompText::StoreToMem(const ur::Device& dev, n2::CompText& comp) const
{
	comp.SetText(m_text);
}

void N2CompText::LoadFromMem(const n2::CompText& comp)
{
	m_text = comp.GetText();
}

size_t N2CompText::GetTextboxBinSize(const pt2::Textbox& tb)
{
	size_t sz = 0;

	sz += sizeof(uint16_t);		// width
	sz += sizeof(uint16_t);		// height

	sz += sizeof(uint8_t);		// font_type
	sz += sizeof(uint8_t);		// font_size
	sz += GetColorBinSize(tb.font_color);

	sz += sizeof(uint8_t);		// has_edge
	sz += sizeof(uint16_t);		// edge_size
	sz += GetColorBinSize(tb.edge_color);

	sz += sizeof(uint8_t);		// align_hori
	sz += sizeof(uint8_t);		// align_vert

	sz += sizeof(uint16_t);		// space_hori
	sz += sizeof(uint16_t);		// space_vert

	sz += sizeof(uint8_t);		// overlabel

	sz += sizeof(uint8_t);		// richtext

	return sz;
}

void N2CompText::StoreTextboxToBin(const pt2::Textbox& tb, bs::ExportStream& es)
{
	es.Write(static_cast<uint16_t>(tb.width));
	es.Write(static_cast<uint16_t>(tb.height));

	es.Write(static_cast<uint8_t>(tb.font_type));
	es.Write(static_cast<uint8_t>(tb.font_size));
	StoreColorToBin(tb.font_color, es);

	es.Write(static_cast<uint8_t>(tb.has_edge));
	es.Write(static_cast<uint16_t>(bs::float2int(tb.edge_size, 1000)));
	StoreColorToBin(tb.edge_color, es);

	es.Write(static_cast<uint8_t>(tb.align_hori));
	es.Write(static_cast<uint8_t>(tb.align_vert));

	es.Write(static_cast<uint16_t>(bs::float2int(tb.space_hori, 1000)));
	es.Write(static_cast<uint16_t>(bs::float2int(tb.space_vert, 1000)));

	es.Write(static_cast<uint8_t>(tb.overlabel));

	es.Write(static_cast<uint8_t>(tb.richtext));
}

void N2CompText::LoadTextboxFromBin(pt2::Textbox& tb, bs::ImportStream& is)
{
	tb.width  = is.UInt16();
	tb.height = is.UInt16();

	tb.font_type = is.UInt8();
	tb.font_size = is.UInt8();
	LoadColorFromBin(tb.font_color, is);

	tb.has_edge = is.UInt8();
	tb.edge_size = bs::int2float(is.UInt16(), 1000);
	LoadColorFromBin(tb.edge_color, is);

	tb.align_hori = static_cast<pt2::Textbox::HoriAlign>(is.UInt8());
	tb.align_vert = static_cast<pt2::Textbox::VertAlign>(is.UInt8());

	tb.space_hori = bs::int2float(is.UInt16(), 1000);
	tb.space_vert = bs::int2float(is.UInt16(), 1000);

	tb.overlabel = static_cast<pt2::Textbox::OverLabel>(is.UInt8());

	tb.richtext = is.UInt8();
}

void N2CompText::StoreTextboxToJson(const pt2::Textbox& tb, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
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

void N2CompText::LoadTextboxFromJson(pt2::Textbox& tb, mm::LinearAllocator& alloc, const rapidjson::Value& val)
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

size_t N2CompText::GetColorBinSize(const pt2::GradientColor& col)
{
	size_t sz = 0;
	sz += sizeof(uint8_t);		// items num
	sz += sizeof(float);		// angle
	sz += (sizeof(uint32_t) + sizeof(float)) * col.items.size();
	return sz;
}

void N2CompText::StoreColorToBin(const pt2::GradientColor& col, bs::ExportStream& es)
{
	GD_ASSERT(col.items.size() < std::numeric_limits<uint8_t>::max(), "overoflow");
	uint8_t num = static_cast<uint8_t>(col.items.size());
	es.Write(num);

	float angle = col.angle;
	es.Write(angle);

	for (auto& item : col.items)
	{
		uint32_t col = item.col.ToRGBA();
		es.Write(col);

		float pos = item.pos;
		es.Write(pos);
	}
}

void N2CompText::LoadColorFromBin(pt2::GradientColor& col, bs::ImportStream& is)
{
	size_t num = is.UInt8();
	col.items.resize(num);

	col.angle = is.Float();

	for (size_t i = 0; i < num; ++i)
	{
		auto& item = col.items[i];
		item.col.FromRGBA(is.UInt32());
		item.pos = is.Float();
	}
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