#include "ns/N3CompTransform.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node3/CompTransform.h>

namespace
{

static const uint8_t POSITION_MASK		= 0x01;
static const uint8_t ANGLE_MASK			= 0x02;
static const uint8_t SCALE_MASK			= 0x04;

}

namespace ns
{

N3CompTransform::N3CompTransform()
{
	m_scale.Set(1, 1, 1);
}

size_t N3CompTransform::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);		// type

	if (m_position != sm::vec3(0, 0, 0)) {
		sz += sizeof(float) * 3;
	}
	if (m_angle != sm::Quaternion()) {
		sz += sizeof(float) * 4;
	}
	if (m_scale != sm::vec3(1, 1, 1)) {
		sz += sizeof(float) * 3;
	}

	return sz;
}

void N3CompTransform::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{

	uint8_t type = 0;
	if (m_position != sm::vec3(0, 0, 0)) {
		type |= POSITION_MASK;
	}
	if (m_angle != sm::Quaternion()) {
		type |= ANGLE_MASK;
	}
	if (m_scale != sm::vec3(1, 1, 1)) {
		type |= SCALE_MASK;
	}
	es.Write(type);

	if (type & POSITION_MASK)
	{
		es.Write(static_cast<float>(m_position.x));
		es.Write(static_cast<float>(m_position.y));
		es.Write(static_cast<float>(m_position.z));
	}
	if (type & ANGLE_MASK)
	{
		es.Write(static_cast<float>(m_angle.x));
		es.Write(static_cast<float>(m_angle.y));
		es.Write(static_cast<float>(m_angle.z));
		es.Write(static_cast<float>(m_angle.w));
	}
	if (type & SCALE_MASK)
	{
		es.Write(static_cast<float>(m_scale.x));
		es.Write(static_cast<float>(m_scale.y));
		es.Write(static_cast<float>(m_scale.z));
	}
}

void N3CompTransform::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	size_t type = is.UInt8();
	if (type & POSITION_MASK)
	{
		m_position.x = is.Float();
		m_position.y = is.Float();
		m_position.z = is.Float();
	}
	if (type & ANGLE_MASK)
	{
		m_angle.x = is.Float();
		m_angle.y = is.Float();
		m_angle.z = is.Float();
		m_angle.w = is.Float();
	}
	if (type & SCALE_MASK)
	{
		m_scale.x = is.Float();
		m_scale.y = is.Float();
		m_scale.z = is.Float();
	}
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

void N3CompTransform::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
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

void N3CompTransform::StoreToMem(const ur::Device& dev, n3::CompTransform& comp) const
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