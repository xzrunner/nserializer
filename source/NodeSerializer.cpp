#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"

#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>
#include <node0/CompAsset.h>
#include <node0/NodeSharedComp.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>

#include <boost/filesystem.hpp>

namespace ns
{

bool NodeSerializer::StoreNodeToJson(const n0::SceneNodePtr& node, const std::string& dir,
                                     rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)
{
	bool ret = false;

	val.SetArray();

	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeSharedComp>& comp)->bool 
	{	
		if (comp->TypeID() == n0::GetSharedCompTypeID<n0::CompAsset>())
		{
			rapidjson::Value cval;
			cval.SetObject();
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			auto& filepath = ceditor.GetFilepath();
			if (!filepath.empty())
			{
				std::string relative = boost::filesystem::relative(ceditor.GetFilepath(), dir).string();
				cval.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
				val.PushBack(cval, alloc);
				ret = true;
				return true;
			}
		}

		rapidjson::Value cval;
		if (CompSerializer::Instance()->ToJson(*comp, dir, cval, alloc))		
		{
			val.PushBack(cval, alloc);
			ret = true;
		}
		return true;
	});

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeUniqueComp>& comp)->bool 
	{
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