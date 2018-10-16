#include "ns/N2CompShape.h"
#include "ns/CompType.h"

#include <node2/CompShape.h>
#include <geoshape/Shape.h>

#include <boost/filesystem.hpp>
#include <rttr/type.h>

namespace ns
{

size_t N2CompShape::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompShape::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompShape::LoadFromBin(const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompShape::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("type", rapidjson::Value(m_type.c_str(), alloc), alloc);
}

void N2CompShape::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_type = val["type"].GetString();
}

void N2CompShape::StoreToMem(n2::CompShape& comp) const
{
	auto type = rttr::type::get_by_name(m_type).create();
	if (type.is_valid()) {
		auto shape = type.get_value<std::shared_ptr<gs::Shape>>();
		comp.SetShape(shape);
	}
}

void N2CompShape::LoadFromMem(const n2::CompShape& comp)
{
	auto& shape = comp.GetShape();
	m_type = shape->get_type().get_name().to_string();
}

}