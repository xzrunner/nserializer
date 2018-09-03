#pragma once

#include <cu/cu_macro.h>

#include <map>
#include <string>

namespace ns
{

class CompIdxMgr
{
public:
	size_t CompTypeName2Idx(const std::string& name);

	void AddExtTypeToIdx(const std::string& name, size_t idx);

private:
	std::map<std::string, size_t> m_ext_name2idx;

	size_t m_next_idx;

	CU_SINGLETON_DECLARATION(CompIdxMgr);

}; // CompIdxMgr

}