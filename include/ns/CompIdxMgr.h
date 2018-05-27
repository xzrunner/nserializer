#pragma once

#include "ns/CompIdx.h"

#include <cu/cu_macro.h>

#include <map>
#include <string>

namespace ns
{

class CompIdxMgr
{
public:
	CompIdx CompTypeName2Idx(const std::string& name);

	void AddExtTypeToIdx(const std::string& name, CompIdx idx) {
		m_ext_name2idx.insert({ name, idx });
	}

private:
	std::map<std::string, CompIdx> m_ext_name2idx;

	CU_SINGLETON_DECLARATION(CompIdxMgr);

}; // CompIdxMgr

}