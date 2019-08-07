#pragma once

#include <cu/cu_macro.h>

#include <functional>

namespace ns
{

class Blackboard
{
public:
	void SetGenNodeIdFunc(std::function<uint32_t()> func) {
		m_gen_node_id_func = func;
	}

    uint32_t GenNodeId() const;

private:
	std::function<uint32_t()> m_gen_node_id_func = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}