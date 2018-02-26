#include "ns/N2CompTransform.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node2/CompTransform.h>

namespace ns
{

N2CompTransform::N2CompTransform()
{
	m_pos_x = m_pos_y = 0;
	m_angle = 0;
	m_scale_x = m_scale_y = 1;
	m_shear_x = m_shear_y = 0;
}

size_t N2CompTransform::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);   // type
	
	sz += sizeof(float) * 2; // pos
	sz += sizeof(float);     // angle
	sz += sizeof(float) * 2; // scale
	sz += sizeof(float) * 2; // shear

	return sz;
}

void N2CompTransform::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(COMP_TRANSFORM)); // type

	es.Write(static_cast<float>(m_pos_x));          // pos
	es.Write(static_cast<float>(m_pos_y));

	es.Write(static_cast<float>(m_angle));          // angle

	es.Write(static_cast<float>(m_scale_x));        // scale
	es.Write(static_cast<float>(m_scale_y));

	es.Write(static_cast<float>(m_shear_x));        // shear
	es.Write(static_cast<float>(m_shear_y));
}

void N2CompTransform::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	m_pos_x = is.Float();
	m_pos_y = is.Float();

	m_angle = is.Float();

	m_scale_x = is.Float();
	m_scale_y = is.Float();

	m_shear_x = is.Float();
	m_shear_y = is.Float();
}

void N2CompTransform::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("pos_x", m_pos_x, alloc);
	val.AddMember("pos_y", m_pos_y, alloc);

	val.AddMember("angle", m_angle, alloc);

	val.AddMember("scale_x", m_scale_x, alloc);
	val.AddMember("scale_y", m_scale_y, alloc);

	val.AddMember("shear_x", m_shear_x, alloc);
	val.AddMember("shear_y", m_shear_y, alloc);
}

void N2CompTransform::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_pos_x = val["pos_x"].GetFloat();
	m_pos_y = val["pos_y"].GetFloat();

	m_angle = val["angle"].GetFloat();

	m_scale_x = val["scale_x"].GetFloat();
	m_scale_y = val["scale_y"].GetFloat();

	m_shear_x = val["shear_x"].GetFloat();
	m_shear_y = val["shear_y"].GetFloat();
}

void N2CompTransform::StoreToMem(n2::CompTransform& comp) const
{
	pt2::SRT srt;
	srt.position.Set(m_pos_x, m_pos_y);
	srt.angle = m_angle;
	srt.scale.Set(m_scale_x, m_scale_y);
	srt.shear.Set(m_shear_x, m_shear_y);
	srt.Update();

	comp.GetTrans().SetSRT(srt);
}

void N2CompTransform::LoadFromMem(const n2::CompTransform& comp)
{
	auto& srt = comp.GetTrans().GetSRT();

	m_pos_x = srt.position.x;
	m_pos_y = srt.position.y;

	m_angle = srt.angle;

	m_scale_x = srt.scale.x;
	m_scale_y = srt.scale.y;

	m_shear_x = srt.shear.x;
	m_shear_y = srt.shear.y;
}

}