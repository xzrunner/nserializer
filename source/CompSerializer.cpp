#include "ns/CompSerializer.h"
#include "ns/CompIdxMgr.h"
#include "ns/NodeFactory.h"

#include <guard/check.h>
#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/NodeComp.h>

#include <boost/filesystem.hpp>

namespace ns
{

CU_SINGLETON_DEFINITION(CompSerializer);

const char* CompSerializer::COMP_TYPE_NAME = "comp_type";

CompSerializer::CompSerializer()
{
}

void CompSerializer::AddToJsonFunc(const std::string& name, const ToJsonFunc& func)
{
	auto status = m_to_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

void CompSerializer::AddFromJsonFunc(const std::string& name, const FromJsonFunc& func)
{
	auto status = m_from_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

bool CompSerializer::ToJson(const n0::NodeComp& comp,
	                        const std::string& dir,
	                        rapidjson::Value& val,
	                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
	auto itr = m_to_json.find(comp.Type());
	if (itr != m_to_json.end())
	{
		val.SetObject();
		bool ret = itr->second(comp, dir, val, alloc);
		if (ret) {
			val.AddMember(rapidjson::StringRef(COMP_TYPE_NAME),
				rapidjson::StringRef(comp.Type()), alloc);
		}
		return ret;
	}
	else
	{
		GD_REPORT_ASSERT("no comp creator");
		return false;
	}
}

void CompSerializer::FromJson(n0::NodeComp& comp,
	                          const std::string& dir,
	                          const rapidjson::Value& val) const
{
	auto type = val[COMP_TYPE_NAME].GetString();
	auto itr = m_from_json.find(type);
	if (itr != m_from_json.end()) {
		itr->second(comp, dir, val);
	} else {
		GD_REPORT_ASSERT("no comp creator");
	}
}

void CompSerializer::AddGetBinSizeFunc(const std::string& name, const GetBinSizeFunc& func)
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(name);
	m_get_bin_sz[idx] = func;
}

void CompSerializer::AddToBinFunc(const std::string& name, const ToBinFunc& func)
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(name);
	m_to_bin[idx] = func;
}

void CompSerializer::AddFromBinFunc(const std::string& name, const FromBinFunc& func)
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(name);
	m_from_bin[idx] = func;
}

size_t CompSerializer::GetBinSize(const n0::NodeComp& comp, const std::string& dir) const
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(comp.Type());
	size_t comp_sz = m_get_bin_sz[idx](comp, dir);
	if (comp_sz == 0) {
		return 0;
	} else {
		// comp type idx
		return sizeof(uint8_t) + comp_sz;
	}
}

void CompSerializer::ToBin(const n0::NodeComp& comp, const std::string& dir, bs::ExportStream& es) const
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(comp.Type());
	size_t comp_sz = m_get_bin_sz[idx](comp, dir);
	if (comp_sz != 0) {
		es.Write(static_cast<uint8_t>(idx));
		m_to_bin[idx](comp, dir, es);
	}
}

void CompSerializer::FromBin(n0::NodeComp& comp, const std::string& dir, bs::ImportStream& is, CompIdx idx) const
{
	size_t comp_idx;
	if (idx != COMP_INVALID) {
		comp_idx = idx;
	} else {
		comp_idx = is.UInt8();
	}
	m_from_bin[comp_idx](comp, dir, is);
}

}