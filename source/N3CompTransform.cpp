#include "ns/N3CompTransform.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node3/CompTransform.h>

namespace ns
{

size_t N3CompTransform::GetBinSize(const std::string& dir) const
{
	// todo
	return 0;
}

void N3CompTransform::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N3CompTransform::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N3CompTransform::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();
	
	if (m_position != sm::vec3(0, 0, 0))
	{
		rapidjson::Value val_pos;
		val_pos.SetArray();
		val_pos.PushBack(m_position.x, alloc);
		val_pos.PushBack(m_position.y, alloc);
		val_pos.PushBack(m_position.z, alloc);
		val.AddMember("position", val_pos, alloc);
	}
	
	if (m_angle != sm::Quaternion())
	{
		rapidjson::Value val_angle;
		val_angle.SetArray();
		float roll, pitch, yaw;
		sm::Quaternion::TransToEulerAngle(m_angle, roll, pitch, yaw);
		val_angle.PushBack(pitch, alloc);
		val_angle.PushBack(yaw, alloc);
		val_angle.PushBack(roll, alloc);
		val.AddMember("angle", val_angle, alloc);
	}
	
	if (m_scale != sm::vec3(1, 1, 1))
	{
		rapidjson::Value val_scale;
		val_scale.SetArray();
		val_scale.PushBack(m_scale.x, alloc);
		val_scale.PushBack(m_scale.y, alloc);
		val_scale.PushBack(m_scale.z, alloc);
		val.AddMember("scale", val_scale, alloc);
	}
}

void N3CompTransform::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	if (val.HasMember("position"))
	{
		m_position.x = val["position"][0].GetFloat();
		m_position.y = val["position"][1].GetFloat();
		m_position.z = val["position"][2].GetFloat();
	}
	
	if (val.HasMember("angle"))
	{
		float pitch = val["angle"][0].GetFloat();
		float yaw   = val["angle"][1].GetFloat();
		float roll  = val["angle"][2].GetFloat();
		m_angle = sm::Quaternion::CreateFromEulerAngle(roll, pitch, yaw);
	}
	
	if (val.HasMember("scale"))
	{
		m_scale.x = val["scale"][0].GetFloat();
		m_scale.y = val["scale"][1].GetFloat();
		m_scale.z = val["scale"][2].GetFloat();
	}
}

void N3CompTransform::StoreToMem(const n0::SceneNodePtr& node, n3::CompTransform& comp) const
{
	comp.SetPosition(m_position);
	comp.SetAngle(m_angle);
	comp.SetScale(m_scale);
}

void N3CompTransform::LoadFromMem(const n3::CompTransform& comp)
{
	m_position = comp.GetPosition();
	m_angle = comp.GetAngle();
	m_scale = comp.GetScale();
}

}