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

	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeSharedComp>& comp)->bool {
		rapidjson::Value cval;
		if (CompSerializer::Instance()->ToJson(*comp, dir, cval, alloc))
		{
			val.PushBack(cval, alloc);
			ret = true;
		}
		return true;
	});

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeUniqueComp>& comp)->bool {
		rapidjson::Value cval;
		if (CompSerializer::Instance()->ToJson(*comp, dir, cval, alloc))
		{
			val.PushBack(cval, alloc);
			ret = true;
		}
		return true;
	});

	return ret;
}

bool NodeSerializer::LoadNodeFromJson(n0::SceneNodePtr& node,
	                                  const std::string& dir,
	                                  const rapidjson::Value& val)
{
	for (auto itr = val.Begin(); itr != val.End(); ++itr) {
		CompSerializer::Instance()->FromJson(node, dir, *itr);
	}

	if (node->HasUniqueComp<n2::CompBoundingBox>() &&
		node->HasUniqueComp<n2::CompTransform>())
	{
		auto& cbb = node->GetUniqueComp<n2::CompBoundingBox>();
		auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
		cbb.Build(ctrans.GetTrans().GetSRT());
	}

	return !val.Empty();
}

}