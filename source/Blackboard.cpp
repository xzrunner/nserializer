#include "ns/Blackboard.h"

namespace ns
{

CU_SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
{
}

uint32_t Blackboard::GenNodeId() const
{
    return m_gen_node_id_func ? m_gen_node_id_func() : 0;
}

}