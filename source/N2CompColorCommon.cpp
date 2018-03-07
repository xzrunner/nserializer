#include "ns/N2CompColorCommon.h"
#include "ns/CompType.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <bs/FixedPointNum.h>
#include <node2/CompColorCommon.h>

namespace ns
{

N2CompColorCommon::N2CompColorCommon()
	: m_mul(0)
	, m_add(0)
{
}

size_t N2CompColorCommon::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);  // type
	
	sz += sizeof(uint32_t);	// mul
	sz += sizeof(uint32_t);	// add

	return sz;
}

void N2CompColorCommon::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(COMP_COLOR_COMMON)); // type

	es.Write(static_cast<uint32_t>(m_mul));            // mul
	es.Write(static_cast<uint32_t>(m_add));            // add
}

void N2CompColorCommon::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	m_mul = is.UInt32();
	m_add = is.UInt32();
}

void N2CompColorCommon::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("mul", m_mul, alloc);
	val.AddMember("add", m_add, alloc);
}

void N2CompColorCommon::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_mul = val["mul"].GetUint();
	m_add = val["add"].GetUint();
}

void N2CompColorCommon::StoreToMem(const n0::SceneNode& node, n2::CompColorCommon& comp) const
{
	pt2::RenderColorCommon col;
	col.mul.FromRGBA(m_mul);
	col.add.FromRGBA(m_add);

	comp.SetColor(col);
}

void N2CompColorCommon::LoadFromMem(const n2::CompColorCommon& comp)
{
	auto& col = comp.GetColor();
	m_mul = col.mul.ToRGBA();
	m_add = col.add.ToRGBA();
}

}