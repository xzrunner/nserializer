#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>

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
		if (CompSerializer::Instance()->ToJson(*comp, dir, cval, alloc)) 
		{
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
	for (auto itr = val.Begin(); itr != val.End(); ++itr) {
		CompSerializer::Instance()->FromJson(node, dir, *itr);
	}

	if (node->HasComponent<n2::CompBoundingBox>() &&
		node->HasComponent<n2::CompTransform>())
	{
		auto& cbb = node->GetComponent<n2::CompBoundingBox>();
		auto& ctrans = node->GetComponent<n2::CompTransform>();
		cbb.Build(ctrans.GetTrans().GetSRT());
	}

	return !val.Empty();
}

}