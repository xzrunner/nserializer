#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"
#include "ns/CompFactory.h"
#include "ns/CompIdxMgr.h"

#include <js/RapidJsonHelper.h>
#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/SceneNode.h>
#include <node0/CompAsset.h>
#include <node0/CompIdentity.h>
#include <node0/NodeComp.h>

#include <boost/filesystem.hpp>

namespace
{

static const uint8_t COMP_END_FALG = 0xff;

}

namespace ns
{

bool NodeSerializer::StoreToJson(const n0::SceneNodePtr& node, const std::string& dir, rapidjson::Value& val,
                                 rapidjson::MemoryPoolAllocator<>& alloc, bool skip_asset)
{
	bool ret = false;

	val.SetArray();

	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeComp>& comp)->bool
	{
		// skip CompAsset, use CompIdentity to create CompAsset
		// here only store children recursively
		if (!skip_asset && comp->TypeID() == n0::GetCompTypeID<n0::CompAsset>())
		{
			auto& cid = node->GetUniqueComp<n0::CompIdentity>();
			auto& filepath = cid.GetFilepath();
			if (!filepath.empty())
			{
				auto ext = boost::filesystem::extension(filepath);
				std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
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

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeComp>& comp)->bool
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

bool NodeSerializer::LoadFromJson(const ur2::Device& dev, n0::SceneNodePtr& node,
                                  const std::string& dir, const rapidjson::Value& val)
{
	for (auto itr = val.Begin(); itr != val.End(); ++itr)
	{
		auto& cval = *itr;

		auto type_name = cval[CompSerializer::COMP_TYPE_NAME].GetString();
		auto type_idx = CompIdxMgr::Instance()->CompTypeName2Idx(type_name);
		auto& comp = CompFactory::Instance()->Create(node, type_idx);
		CompSerializer::Instance()->FromJson(dev, comp, dir, *itr);

		if (type_idx == CompIdx::COMP_N0_ID)
		{
			auto& cid = static_cast<n0::CompIdentity&>(comp);
			auto casset = CompFactory::Instance()->CreateAsset(dev, cid.GetFilepath());
			if (casset) {
				GD_ASSERT(!node->HasSharedComp<n0::CompAsset>(), "already has asset");
				node->AddSharedCompNoCreate(casset);
			}
		}
	}
	return !val.Empty();
}

size_t NodeSerializer::GetBinSize(const n0::SceneNodePtr& node, const std::string& dir)
{
	size_t sz = 0;

	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeComp>& comp)->bool
	{
		if (comp->TypeID() != n0::GetCompTypeID<n0::CompAsset>()) {
			sz += CompSerializer::Instance()->GetBinSize(*comp, dir);
		}
		return true;
	});

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeComp>& comp)->bool
	{
		sz += CompSerializer::Instance()->GetBinSize(*comp, dir);
		return true;
	});

	sz += sizeof(uint8_t);		// end flag

	return sz;
}

void NodeSerializer::StoreToBin(const n0::SceneNodePtr& node, const std::string& dir, bs::ExportStream& es)
{
	node->TraverseSharedComp([&](const std::shared_ptr<n0::NodeComp>& comp)->bool
	{
		if (comp->TypeID() != n0::GetCompTypeID<n0::CompAsset>()) {
			CompSerializer::Instance()->ToBin(*comp, dir, es);
		}
		return true;
	});

	node->TraverseUniqueComp([&](const std::unique_ptr<n0::NodeComp>& comp)->bool
	{
		CompSerializer::Instance()->ToBin(*comp, dir, es);
		return true;
	});

	es.Write(COMP_END_FALG);
}

void NodeSerializer::LoadFromBin(const ur2::Device& dev, n0::SceneNodePtr& node,
                                 const std::string& dir, bs::ImportStream& is)
{
	while (true)
	{
		uint8_t type = is.UInt8();
		if (type == COMP_END_FALG) {
			break;
		}

		CompIdx idx = static_cast<CompIdx>(type);
		auto& comp = CompFactory::Instance()->Create(node, idx);
		CompSerializer::Instance()->FromBin(dev, comp, dir, is, idx);

		if (idx == CompIdx::COMP_N0_ID)
		{
			auto& cid = static_cast<n0::CompIdentity&>(comp);
			auto casset = CompFactory::Instance()->CreateAsset(dev, cid.GetFilepath());
			GD_ASSERT(!node->HasSharedComp<n0::CompAsset>(), "already has asset");
			node->AddSharedCompNoCreate(casset);
		}
	}
}

bool NodeSerializer::StoreAssetCompToJson(const n0::NodeComp& comp, const std::string& filepath)
{
	rapidjson::Document doc;

	auto dir = boost::filesystem::path(filepath).parent_path().string();
	bool ret = CompSerializer::Instance()->ToJson(comp, dir, doc, doc.GetAllocator());

	js::RapidJsonHelper::WriteToFile(filepath.c_str(), doc);

	return ret;
}

}