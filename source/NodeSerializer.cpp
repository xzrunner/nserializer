#include "ns/NodeSerializer.h"

#include <node0/SceneNode.h>
#include <node0/CompStorer.h>
#include <node0/CompFactory.h>

namespace ns
{

bool NodeSerializer::StoreNodeToJson(const n0::SceneNodePtr& node, const std::string& dir,
                                     rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
{
	bool ret = false;

	val.SetArray();

	auto& components = node->GetAllComponents();
	for (auto& comp : components)
	{
		rapidjson::Value cval;
		if (n0::CompStorer::Instance()->ToJson(*comp, dir, cval, alloc)) 
		{
			cval.AddMember("type", rapidjson::StringRef(comp->Type()), alloc);
			val.PushBack(cval, alloc);
			ret = true;
		}
	}

	return ret;
}

bool NodeSerializer::LoadNodeFromJson(n0::SceneNodePtr& node,
	                                  const std::string& dir,
	                                  const rapidjson::Value& val)
{
	for (auto itr = val.Begin(); itr != val.End(); ++itr)
	{
		n0::CompFactory::Instance()->Create(
			node, (*itr)["type"].GetString(), dir, *itr);
	}
	return !val.Empty();
}

}