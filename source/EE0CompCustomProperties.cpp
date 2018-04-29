#include "ns/EE0CompCustomProperties.h"

#include <SM_Vector.h>
#include <painting2/Color.h>
#include <guard/check.h>

namespace ns
{

size_t EE0CompCustomProperties::GetBinSize(const std::string& dir) const
{
	return 0;
}

void EE0CompCustomProperties::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
}

void EE0CompCustomProperties::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
}

void EE0CompCustomProperties::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value props_val;
	props_val.SetArray();
	for (auto& prop : m_props)
	{
		rapidjson::Value prop_val;
		prop_val.SetObject();

		prop_val.AddMember("key", rapidjson::Value(prop.key.c_str(), alloc), alloc);

		switch (prop.type)
		{
		case ee0::CompCustomProperties::PROP_BOOL:
			prop_val.AddMember("type", "bool", alloc);
			prop_val.AddMember("val", prop.val.m_val.bl, alloc);
			break;
		case ee0::CompCustomProperties::PROP_INT:
			prop_val.AddMember("type", "int", alloc);
			prop_val.AddMember("val", prop.val.m_val.l, alloc);
			break;
		case ee0::CompCustomProperties::PROP_FLOAT:
			prop_val.AddMember("type", "float", alloc);
			prop_val.AddMember("val", prop.val.m_val.flt, alloc);
			break;
		case ee0::CompCustomProperties::PROP_STRING:
			prop_val.AddMember("type", "string", alloc);
			prop_val.AddMember("val", rapidjson::Value(prop.val.m_val.pc, alloc), alloc);
			break;
		case ee0::CompCustomProperties::PROP_VEC2:
			{
				prop_val.AddMember("type", "vec2", alloc);

				auto& vec2 = *static_cast<sm::vec2*>(prop.val.m_val.pv);
				rapidjson::Value vec2_val;
				vec2_val.SetObject();
				vec2_val.AddMember("x", vec2.x, alloc);
				vec2_val.AddMember("y", vec2.y, alloc);
				prop_val.AddMember("val", vec2_val, alloc);
			}
			break;
		case ee0::CompCustomProperties::PROP_COLOR:
			{
				prop_val.AddMember("type", "color", alloc);

				auto& col = *static_cast<pt2::Color*>(prop.val.m_val.pv);
				prop_val.AddMember("val", col.ToRGBA(), alloc);
			}
			break;
		}
		props_val.PushBack(prop_val, alloc);
	}

	val.AddMember("props", props_val, alloc);
}

void EE0CompCustomProperties::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	for (auto& prop_val : val["props"].GetArray())
	{
		auto type = std::string(prop_val["type"].GetString());
		auto key  = prop_val["key"].GetString();
		auto& val = prop_val["val"];

		ee0::CompCustomProperties::Property prop;
		prop.key = key;
		if (type == "bool")
		{
			prop.type = ee0::CompCustomProperties::PROP_BOOL;
			prop.val.m_type = ee0::VT_BOOL;
			prop.val.m_val.bl = val.GetBool();
		}
		else if (type == "int")
		{
			prop.type = ee0::CompCustomProperties::PROP_INT;
			prop.val.m_type = ee0::VT_LONG;
			prop.val.m_val.l = val.GetInt();
		}
		else if (type == "float")
		{
			prop.type = ee0::CompCustomProperties::PROP_FLOAT;
			prop.val.m_type = ee0::VT_FLOAT;
			prop.val.m_val.flt = val.GetFloat();
		}
		else if (type == "string")
		{
			prop.type = ee0::CompCustomProperties::PROP_STRING;
			prop.val.m_type = ee0::VT_PCHAR;

			auto str = val.GetString();
			char* tmp = new char[strlen(str) + 1];
			strcpy(tmp, str);
			prop.val.m_val.pc = tmp;
		}
		else if (type == "vec2")
		{
			prop.type = ee0::CompCustomProperties::PROP_VEC2;
			prop.val.m_type = ee0::VT_PVOID;

			auto x = val["x"].GetFloat();
			auto y = val["y"].GetFloat();
			auto tmp = new sm::vec2(x, y);
			prop.val.m_val.pv = tmp;
		}
		else if (type == "color")
		{
			prop.type = ee0::CompCustomProperties::PROP_COLOR;
			prop.val.m_type = ee0::VT_PVOID;

			uint32_t rgba = val.GetUint();
			auto tmp = new pt2::Color();
			tmp->FromRGBA(rgba);
			prop.val.m_val.pv = tmp;
		}
		else
		{
			GD_REPORT_ASSERT("unknown type.");
		}

		m_props.push_back(prop);
	}
}

void EE0CompCustomProperties::StoreToMem(ee0::CompCustomProperties& comp) const
{
	for (auto& prop : m_props) {
		comp.Add(prop);
	}
}

void EE0CompCustomProperties::LoadFromMem(const ee0::CompCustomProperties& comp)
{
	m_props = comp.GetAllProp();
}

}