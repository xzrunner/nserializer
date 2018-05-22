#pragma once

#include "ns/CompSerializer.h"

#include <node0/SceneNode.h>
#include <memmgr/LinearAllocator.h>

namespace ns
{

template <typename TComp, typename TCompNS>
void RegistCallback::AddUniqueCB()
{
	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)
		{
			auto& comp = node->HasUniqueComp<TComp>() ?
			node->GetUniqueComp<TComp>() : node->AddUniqueComp<TComp>();
			TCompNS ser;
			mm::LinearAllocator alloc;
			ser.LoadFromJson(alloc, dir, val);
			ser.StoreToMem(comp);
		});
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir,
			rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
		{
			TCompNS seri;
			seri.LoadFromMem(static_cast<const TComp&>(comp));
			seri.StoreToJson(dir, val, alloc);
			return true;
		});
}

template <typename TComp, typename TCompNS>
void RegistCallback::AddSharedCB()
{
	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)
        {
			auto& comp = node->HasSharedComp<TComp>() ?
				node->GetSharedComp<TComp>() : node->AddSharedComp<TComp>();
			TCompNS ser;
			mm::LinearAllocator alloc;
			ser.LoadFromJson(alloc, dir, val);
			ser.StoreToMem(comp);
		}
	);
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeSharedComp& comp, const std::string& dir,
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
		{
			TCompNS seri;
			seri.LoadFromMem(static_cast<const TComp&>(comp));
			seri.StoreToJson(dir, val, alloc);
			return true;
		}
	);
}

template <typename TComp, typename TCompNS>
void RegistCallback::AddUniqueNullCB()
{
	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)
        {
		}
	);
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir,
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
		{
			return false;
		}
	);
}

template <typename TComp, typename TCompNS>
void RegistCallback::AddAssetCB()
{
	CompSerializer::Instance()->AddAssetFromJsonFunc(TComp::TYPE_NAME,
		[](const std::string& dir, const rapidjson::Value& val) -> n0::CompAssetPtr
        {
			auto comp = std::make_shared<TComp>();
			TCompNS ser;
			mm::LinearAllocator alloc;
			ser.LoadFromJson(alloc, dir, val);
			ser.StoreToMem(*comp);
			return comp;
		}
	);
}

}