#include "ns/EditOpSerializer.h"

#include <guard/check.h>

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
	if (type == n2::SetTransformMatOp::TYPE_NAME) {
		type_id = n2::EditOpID::SetTransformMatOp;
	} else if (type == n2::SetPositionOp::TYPE_NAME) {
		type_id = n2::EditOpID::SetPositionOp;
	} else if (type == n2::SetAngleOp::TYPE_NAME) {
		type_id = n2::EditOpID::SetAngleOp;
	} else if (type == n2::SetScaleOp::TYPE_NAME) {
		type_id = n2::EditOpID::SetScaleOp;
	} else if (type == n2::SetShearOp::TYPE_NAME) {
		type_id = n2::EditOpID::SetShearOp;
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

/************************************************************************/
/* SetTransformMat                                                      */
/************************************************************************/

void EditOpSerializer::ToJsonSetTransformMatOp(const n2::SetTransformMatOp& op, 
	                                           rapidjson::Value& val, 
	                                           rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value mat_val;
	mat_val.SetArray();
	auto& mat = op.mat;
	for (auto& f : mat.x) {
		mat_val.PushBack(f, alloc);
	}
	val.AddMember("mat", mat_val, alloc);
}

std::unique_ptr<n2::EditOp> 
EditOpSerializer::FromJsonSetTransformMatOp(const rapidjson::Value& val)
{
	sm::Matrix2D mat;
	auto& mat_val = val["mat"];
	int ptr = 0;
	for (auto& f : mat_val.GetArray()) {
		mat.x[ptr++] = f.GetFloat();
	}

	return std::make_unique<n2::SetTransformMatOp>(mat);
}

/************************************************************************/
/* SetPosition                                                          */
/************************************************************************/

void EditOpSerializer::ToJsonSetPositionOp(const n2::SetPositionOp& op, 
	                                       rapidjson::Value& val,
	                                       rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value pos_val;
	pos_val.SetArray();
	pos_val.PushBack(op.pos.x, alloc);
	pos_val.PushBack(op.pos.y, alloc);
	val.AddMember("pos", pos_val, alloc);
}

std::unique_ptr<n2::EditOp>
EditOpSerializer::FromJsonSetPositionOp(const rapidjson::Value& val)
{
	sm::vec2 pos;
	auto& pos_val = val["pos"];
	pos.x = pos_val[0].GetFloat();
	pos.y = pos_val[1].GetFloat();

	return std::make_unique<n2::SetPositionOp>(pos);
}

/************************************************************************/
/* SetAngle                                                             */
/************************************************************************/

void EditOpSerializer::ToJsonSetAngleOp(const n2::SetAngleOp& op, 
	                                    rapidjson::Value& val,
	                                    rapidjson::MemoryPoolAllocator<>& alloc)
{
	val.AddMember("angle", op.angle, alloc);
}

std::unique_ptr<n2::EditOp>
EditOpSerializer::FromJsonSetAngleOp(const rapidjson::Value& val)
{
	float angle = val.GetFloat();
	return std::make_unique<n2::SetAngleOp>(angle);
}

/************************************************************************/
/* SetScale                                                          */
/************************************************************************/

void EditOpSerializer::ToJsonSetScaleOp(const n2::SetScaleOp& op, 
	                                    rapidjson::Value& val,
	                                    rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value scale_val;
	scale_val.SetArray();
	scale_val.PushBack(op.scale.x, alloc);
	scale_val.PushBack(op.scale.y, alloc);
	val.AddMember("scale", scale_val, alloc);
}

std::unique_ptr<n2::EditOp>
EditOpSerializer::FromJsonSetScaleOp(const rapidjson::Value& val)
{
	sm::vec2 scale;
	auto& scale_val = val["scale"];
	scale.x = scale_val[0].GetFloat();
	scale.y = scale_val[1].GetFloat();

	return std::make_unique<n2::SetScaleOp>(scale);
}

/************************************************************************/
/* SetShear                                                          */
/************************************************************************/

void EditOpSerializer::ToJsonSetShearOp(const n2::SetShearOp& op, 
	                                    rapidjson::Value& val,
	                                    rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value shear_val;
	shear_val.SetArray();
	shear_val.PushBack(op.shear.x, alloc);
	shear_val.PushBack(op.shear.y, alloc);
	val.AddMember("shear", shear_val, alloc);
}

std::unique_ptr<n2::EditOp>
EditOpSerializer::FromJsonSetShearOp(const rapidjson::Value& val)
{
	sm::vec2 shear;
	auto& shear_val = val["shear"];
	shear.x = shear_val[0].GetFloat();
	shear.y = shear_val[1].GetFloat();

	return std::make_unique<n2::SetShearOp>(shear);
}

}