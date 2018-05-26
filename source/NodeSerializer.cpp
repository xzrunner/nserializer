#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"

#include <js/RapidJsonHelper.h>
#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/SceneNode.h>
#include <node0/CompAsset.h>
#include <node0/CompIdentity.h>
#include <node0/NodeSharedComp.h>

#include <boost/filesystem.hpp>

namespace ns
{

bool NodeSerializer::StoreToJson(const n0::SceneNodePtr& node, const std::string& dir,
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
			auto& cid = node->GetUniqueComp<n0::CompIdentity>();
			auto& filepath = cid.GetFilepath();
			if (!filepath.empty())
			{
				// store ref
				std::string relative = boost::filesystem::relative(filepath, dir).string();
				cval.AddMember("comp_type", rapidjson::StringRef(comp->Type()), alloc);
				cval.AddMember("comp_path", rapidjson::Value(relative.c_str(), alloc), alloc);
				val.PushBack(cval, alloc);
				ret = true;

				auto ext = boost::filesystem::extension(filepath);
				if (ext == ".json")
				{
					// store to child file
					StoreAssetCompToJson(*comp, filepath);
				}
			}
		}
		else
		{
			rapidjson::Value cval;
			if (CompSerializer::Instance()->ToJson(*comp, dir, cval, alloc))
			{
				val.PushBack(cval, alloc);
				ret = true;
			}
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

bool NodeSerializer::LoadFromJson(n0::SceneNodePtr& node, const std::string& dir,
	                              const rapidjson::Value& val)
{
	for (auto itr = val.Begin(); itr != val.End(); ++itr) {
		CompSerializer::Instance()->FromJson(node, dir, *itr);
	}
	return !val.Empty();
}

size_t NodeSerializer::GetBinSize(const n0::SceneNodePtr& node, const std::string& dir)
{
	size_t sz = 0;

	sz += sizeof(uint8_t);		// comp num

	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeSharedComp>& comp)->bool
	{
		sz += CompSerializer::Instance()->GetBinSize(*comp, dir);
		return true;
	});

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeUniqueComp>& comp)->bool
	{
		sz += CompSerializer::Instance()->GetBinSize(*comp, dir);
		return true;
	});

	return sz;
}

void NodeSerializer::StoreToBin(const n0::SceneNodePtr& node, const std::string& dir, bs::ExportStream& es)
{
	size_t comp_sz = node->GetSharedCompCount() + node->GetUniqueCompCount();
	GD_ASSERT(comp_sz < std::numeric_limits<uint8_t>::max(), "overflow");
	es.Write(static_cast<uint8_t>(comp_sz));

	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeSharedComp>& comp)->bool
	{
		if (comp->TypeID() == n0::GetSharedCompTypeID<n0::CompAsset>())
		{
			uint8_t type_idx = CompSerializer::Instance()->GetTypeIndex(comp->Type());
			es.Write(type_idx);

			// store ref
			std::string relative;
			auto& cid = node->GetUniqueComp<n0::CompIdentity>();
			auto& filepath = cid.GetFilepath();
			if (!filepath.empty()) {
				relative = boost::filesystem::relative(filepath, dir).string();
			}
			es.Write(relative);
		}
		else
		{
			CompSerializer::Instance()->ToBin(*comp, dir, es);
		}
		return true;
	});

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeUniqueComp>& comp)->bool
	{
		CompSerializer::Instance()->ToBin(*comp, dir, es);
		return true;
	});
}

void NodeSerializer::LoadFromBin(n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is)
{
	uint8_t sz = is.UInt8();
	for (size_t i = 0; i < sz; ++i) {
		CompSerializer::Instance()->FromBin(node, dir, is);
	}
}

bool NodeSerializer::StoreAssetCompToJson(const n0::NodeSharedComp& comp, const std::string& filepath)
{
	rapidjson::Document doc;

	auto dir = boost::filesystem::path(filepath).parent_path().string();
	bool ret = CompSerializer::Instance()->ToJson(comp, dir, doc, doc.GetAllocator());

	js::RapidJsonHelper::WriteToFile(filepath.c_str(), doc);

	return ret;
}

}