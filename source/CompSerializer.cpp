#include "ns/CompSerializer.h"

#include <guard/check.h>
#include <node0/NodeComponent.h>

namespace ns
{

CU_SINGLETON_DEFINITION(CompSerializer);

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
	auto status = m_creator.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

bool CompSerializer::ToJson(const n0::NodeComponent& comp, 
	                        const std::string& dir,
	                        rapidjson::Value& val, 
	                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
	auto itr = m_to_json.find(comp.Type());
	if (itr != m_to_json.end()) 
	{
		bool ret = itr->second(comp, dir, val, alloc);
		val.AddMember("type", rapidjson::StringRef(comp.Type()), alloc);
		return ret;
	} 
	else 
	{
		GD_REPORT_ASSERT("no comp creator");
		return false;
	}
}

void CompSerializer::FromJson(n0::SceneNodePtr& node,
	                          const std::string& dir,
	                          const rapidjson::Value& val) const
{
	auto type = val["type"].GetString();
	auto itr = m_creator.find(type);
	if (itr != m_creator.end()) {
		itr->second(node, dir, val);
	} else {
		GD_REPORT_ASSERT("no comp creator");
	}
}

}