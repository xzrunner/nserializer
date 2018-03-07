#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"

#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>
#include <node0/CompAsset.h>
#include <node0/NodeSharedComp.h>

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
				auto ext = boost::filesystem::extension(filepath);
				if (ext == ".json")
				{
					std::string relative = boost::filesystem::relative(ceditor.GetFilepath(), dir).string();
					cval.AddMember("comp_type", rapidjson::StringRef(comp->Type()), alloc);
					cval.AddMember("comp_path", rapidjson::Value(relative.c_str(), alloc), alloc);
					val.PushBack(cval, alloc);
					ret = true;
					return true;
				}
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
	return !val.Empty();
}

}