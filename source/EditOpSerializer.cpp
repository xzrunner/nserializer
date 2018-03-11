#include "ns/EditOpSerializer.h"

#include <guard/check.h>
#include <node2/EditOp.h>

namespace ns
{

#define OP_TO_JSON(Type) \
        [](const n2::EditOp& op, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) \
		{ EditOpSerializer::ToJson##Type(static_cast<const n2::Type&>(op), val, alloc); },
void EditOpSerializer::ToJson(const n2::EditOp& op, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
{
	typedef void(*OpDispatcher) (const n2::EditOp& op, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	static OpDispatcher to_json[] = BUILD_OP_LUT(OP_TO_JSON);

	val.SetObject();

	val.AddMember("op_type", rapidjson::StringRef(GetTypeName(op)), alloc);

	to_json[op.id](op, val, alloc);
}

#define OP_FROM_JSON(Type) \
        [](const rapidjson::Value& val)->std::unique_ptr<n2::EditOp> { return EditOpSerializer::FromJson##Type(val); },
std::unique_ptr<n2::EditOp> EditOpSerializer::FromJson(const rapidjson::Value& val)
{
	typedef std::unique_ptr<n2::EditOp>(*OpDispatcher) (const rapidjson::Value& val);
	static OpDispatcher from_json[] = BUILD_OP_LUT(OP_FROM_JSON);

	uint32_t type_id;
	std::string type = val["op_type"].GetString();
	if (type == n2::SetTransformOp::TYPE_NAME) {
		type_id = n2::EditOpID::SetTransformOp;
	} else {
		GD_REPORT_ASSERT("unknown type");
	}

	return from_json[type_id](val);
}

#define OP_GET_TYPE_NAME(Type) case n2::EditOpID::Type: return n2::##Type::TYPE_NAME;
const char* EditOpSerializer::GetTypeName(const n2::EditOp& op)
{
	switch (op.id)
	{
		MAP_NODE_OPS_BASED_ON_TYPE(OP_GET_TYPE_NAME)
	}
	return nullptr;
}

void EditOpSerializer::ToJsonSetTransformOp(const n2::SetTransformOp& op, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value mat_val;
	mat_val.SetArray();
	for (auto& f : op.mat.x) {
		mat_val.PushBack(f, alloc);
	}
	val.AddMember("mat", mat_val, alloc);
}

std::unique_ptr<n2::EditOp> EditOpSerializer::FromJsonSetTransformOp(const rapidjson::Value& val)
{
	sm::Matrix2D mat;
	auto& mat_val = val["mat"];
	int ptr = 0;
	for (auto& f : mat_val.GetArray()) {
		mat.x[ptr++] = f.GetFloat();
	}

	return std::make_unique<n2::SetTransformOp>(mat);
}

}