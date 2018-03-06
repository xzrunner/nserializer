#include "ns/CompSerializer.h"
#include "ns/NodeFactory.h"

#include <guard/check.h>
#include <node0/NodeUniqueComp.h>
#include <node0/NodeSharedComp.h>

#include <boost/filesystem.hpp>

namespace ns
{

CU_SINGLETON_DEFINITION(CompSerializer);

CompSerializer::CompSerializer()
{
}

void CompSerializer::AddToJsonFunc(const std::string& name, const UniqueToJsonFunc& func)
{
	auto status = m_unique_to_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

void CompSerializer::AddToJsonFunc(const std::string& name, const SharedToJsonFunc& func)
{
	auto status = m_shared_to_json.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

void CompSerializer::AddFromJsonFunc(const std::string& name, const FromJsonFunc& func)
{
	auto status = m_creator.insert(std::make_pair(name, func));
	GD_ASSERT(status.second, "duplicate.");
}

bool CompSerializer::ToJson(const n0::NodeUniqueComp& comp,
	                        const std::string& dir,
	                        rapidjson::Value& val, 
	                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
	auto itr = m_unique_to_json.find(comp.Type());
	if (itr != m_unique_to_json.end())
	{
		bool ret = itr->second(comp, dir, val, alloc);
		val.AddMember("comp_type", rapidjson::StringRef(comp.Type()), alloc);
		return ret;
	} 
	else 
	{
		GD_REPORT_ASSERT("no comp creator");
		return false;
	}
}

bool CompSerializer::ToJson(const n0::NodeSharedComp& comp, 
	                        const std::string& dir, 
	                        rapidjson::Value& val,
	                        rapidjson::MemoryPoolAllocator<>& alloc) const
{
	auto itr = m_shared_to_json.find(comp.Type());
	if (itr != m_shared_to_json.end())
	{
		bool ret = itr->second(comp, dir, val, alloc);
		val.AddMember("comp_type", rapidjson::StringRef(comp.Type()), alloc);
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
	if (val.HasMember("comp_path"))
	{
		auto comp_path = val["comp_path"].GetString();
		auto absolute = boost::filesystem::absolute(comp_path, dir);
		NodeFactory::CreateNodeAssetComp(node, absolute.string());
		return;
	}

	auto type = val["comp_type"].GetString();
	auto itr = m_creator.find(type);
	if (itr != m_creator.end()) {
		itr->second(node, dir, val);
	} else {
		GD_REPORT_ASSERT("no comp creator");
	}
}

}