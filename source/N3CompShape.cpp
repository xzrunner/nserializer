#include "ns/N3CompShape.h"
#include "ns/CompType.h"

#include <node3/CompShape.h>
#include <geoshape/Shape3D.h>
#include <geoshape/Serialize.h>
#include <js/RTTR.h>

#include <rttr/type>
#include <rapidjson/writer.h>

#include <boost/filesystem.hpp>

namespace ns
{

N3CompShape::N3CompShape()
{
    gs::Serialize::Init();
}

size_t N3CompShape::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N3CompShape::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N3CompShape::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N3CompShape::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
    val.SetObject();

    rapidjson::Value shapes_val;
    shapes_val.SetArray();

    for (auto& shape : m_shapes)
    {
        auto str = js::RTTR::ToRapidJson(shape);

        rapidjson::Document doc;
        doc.Parse(str.c_str());

        auto type = shape->get_type().get_name().to_string();
        doc.AddMember("type", rapidjson::Value(type.c_str(), alloc), alloc);

        shapes_val.PushBack(rapidjson::Value(doc, alloc), alloc);
    }

    val.AddMember("shapes", shapes_val, alloc);
}

void N3CompShape::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
    m_shapes.clear();

    auto& shapes = val["shapes"].GetArray();
    m_shapes.reserve(shapes.Size());
    for (auto& val : shapes)
    {
        rapidjson::StringBuffer buffer;

        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        val.Accept(writer);

        rttr::type type = rttr::type::get_by_name(val["type"].GetString());
        assert(type.is_valid());
        rttr::variant var = type.create();
        assert(var.is_valid());

        js::RTTR::FromRapidJson(buffer.GetString(), "", var);

        m_shapes.push_back(var.get_value<std::shared_ptr<gs::Shape3D>>());
    }
}

void N3CompShape::StoreToMem(const ur::Device& dev, n3::CompShape& comp) const
{
    comp.SetShapes(m_shapes);
}

void N3CompShape::LoadFromMem(const n3::CompShape& comp)
{
    m_shapes = comp.GetShapes();
}

}