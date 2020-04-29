#include "ns/N2CompTransform.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node2/CompTransform.h>

namespace
{

static const uint8_t POSITION_MASK		= 0x01;
static const uint8_t ANGLE_MASK			= 0x02;
static const uint8_t SCALE_MASK			= 0x04;
static const uint8_t SHEAR_MASK         = 0x08;

}

namespace ns
{

N2CompTransform::N2CompTransform()
	: m_angle(0)
	, m_scale(1, 1)
{
}

size_t N2CompTransform::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);   // type

	if (m_pos != sm::vec2(0, 0)) {
		sz += sizeof(float) * 2;
	}
	if (m_angle != 0) {
		sz += sizeof(float);
	}
	if (m_scale != sm::vec2(1, 1)) {
		sz += sizeof(float) * 2;
	}
	if (m_shear != sm::vec2(0, 0)) {
		sz += sizeof(float) * 2;
	}

	return sz;
}

void N2CompTransform::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	uint8_t type = 0;
	if (m_pos != sm::vec2(0, 0)) {
		type |= POSITION_MASK;
	}
	if (m_angle != 0) {
		type |= ANGLE_MASK;
	}
	if (m_scale != sm::vec2(1, 1)) {
		type |= SCALE_MASK;
	}
	if (m_shear != sm::vec2(0, 0)) {
		type |= SHEAR_MASK;
	}
	es.Write(type);

	if (type & POSITION_MASK)
	{
		es.Write(static_cast<float>(m_pos.x));
		es.Write(static_cast<float>(m_pos.y));
	}
	if (type & ANGLE_MASK)
	{
		es.Write(static_cast<float>(m_angle));
	}
	if (type & SCALE_MASK)
	{
		es.Write(static_cast<float>(m_scale.x));
		es.Write(static_cast<float>(m_scale.y));
	}
	if (type & SHEAR_MASK)
	{
		es.Write(static_cast<float>(m_shear.x));
		es.Write(static_cast<float>(m_shear.y));
	}
}

void N2CompTransform::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	size_t type = is.UInt8();
	if (type & POSITION_MASK)
	{
		m_pos.x = is.Float();
		m_pos.y = is.Float();
	}
	if (type & ANGLE_MASK)
	{
		m_angle = is.Float();
	}
	if (type & SCALE_MASK)
	{
		m_scale.x = is.Float();
		m_scale.y = is.Float();
	}
	if (type & SHEAR_MASK)
	{
		m_shear.x = is.Float();
		m_shear.y = is.Float();
	}
}

void N2CompTransform::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	if (m_pos != sm::vec2(0, 0))
	{
		rapidjson::Value v;
		v.SetArray();
		v.PushBack(m_pos.x, alloc);
		v.PushBack(m_pos.y, alloc);
		val.AddMember("position", v, alloc);
	}
	if (m_angle != 0) {
		val.AddMember("angle", m_angle, alloc);
	}
	if (m_scale != sm::vec2(1, 1))
	{
		rapidjson::Value v;
		v.SetArray();
		v.PushBack(m_scale.x, alloc);
		v.PushBack(m_scale.y, alloc);
		val.AddMember("scale", v, alloc);
	}
	if (m_shear != sm::vec2(0, 0))
	{
		rapidjson::Value v;
		v.SetArray();
		v.PushBack(m_shear.x, alloc);
		v.PushBack(m_shear.y, alloc);
		val.AddMember("shear", v, alloc);
	}
}

void N2CompTransform::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	if (val.HasMember("position"))
	{
		m_pos.x = val["position"][0].GetFloat();
		m_pos.y = val["position"][1].GetFloat();
	}
	if (val.HasMember("angle"))
	{
		m_angle = val["angle"].GetFloat();
	}
	if (val.HasMember("scale"))
	{
		m_scale.x = val["scale"][0].GetFloat();
		m_scale.y = val["scale"][1].GetFloat();
	}
	if (val.HasMember("shear"))
	{
		m_shear.x = val["shear"][0].GetFloat();
		m_shear.y = val["shear"][1].GetFloat();
	}
}

void N2CompTransform::StoreToMem(const ur::Device& dev, n2::CompTransform& comp) const
{
	pt2::SRT srt;
	srt.position = m_pos;
	srt.angle    = m_angle;
	srt.scale    = m_scale;
	srt.shear    = m_shear;
	srt.Update();
	comp.SetSRT(srt);
}

void N2CompTransform::LoadFromMem(const n2::CompTransform& comp)
{
	auto& srt = comp.GetTrans().GetSRT();
	m_pos   = srt.position;
	m_angle = srt.angle;
	m_scale = srt.scale;
	m_shear = srt.shear;
}

}