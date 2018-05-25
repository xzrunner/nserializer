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
		TCompNS seri;
		mm::LinearAllocator alloc;
		seri.LoadFromJson(alloc, dir, val);
		seri.StoreToMem(comp);
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

	CompSerializer::Instance()->AddGetBinSizeFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir)->size_t
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		return seri.GetBinSize(dir);
	});
	CompSerializer::Instance()->AddFromBinFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is)
	{
		auto& comp = node->HasUniqueComp<TComp>() ?
			node->GetUniqueComp<TComp>() : node->AddUniqueComp<TComp>();
		TCompNS seri;
		seri.LoadFromBin(dir, is);
		seri.StoreToMem(comp);
	});
	CompSerializer::Instance()->AddToBinFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir, bs::ExportStream& es)->bool
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		seri.StoreToBin(dir, es);
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
		TCompNS seri;
		mm::LinearAllocator alloc;
		seri.LoadFromJson(alloc, dir, val);
		seri.StoreToMem(comp);
	});
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeSharedComp& comp, const std::string& dir,
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		seri.StoreToJson(dir, val, alloc);
		return true;
	});

	CompSerializer::Instance()->AddGetBinSizeFunc(TComp::TYPE_NAME,
		[](const n0::NodeSharedComp& comp, const std::string& dir)->size_t
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		return seri.GetBinSize(dir);
	});
	CompSerializer::Instance()->AddFromBinFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is)
	{
		auto& comp = node->HasSharedComp<TComp>() ?
			node->GetSharedComp<TComp>() : node->AddSharedComp<TComp>();
		TCompNS seri;
		seri.LoadFromBin(dir, is);
		seri.StoreToMem(comp);
	});
	CompSerializer::Instance()->AddToBinFunc(TComp::TYPE_NAME,
		[](const n0::NodeSharedComp& comp, const std::string& dir, bs::ExportStream& es)->bool
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		seri.StoreToBin(dir, es);
		return true;
	});
}

template <typename TComp, typename TCompNS>
void RegistCallback::AddUniqueNullCB()
{
	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)
    {
	});
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir,
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	{
		return false;
	});

	CompSerializer::Instance()->AddGetBinSizeFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir)->size_t
	{
		return 0;
	});
	CompSerializer::Instance()->AddFromBinFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node, const std::string& dir, bs::ImportStream& is)
    {
	});
	CompSerializer::Instance()->AddToBinFunc(TComp::TYPE_NAME,
		[](const n0::NodeUniqueComp& comp, const std::string& dir, bs::ExportStream& es)->bool
	{
		return false;
	});
}

template <typename TComp, typename TCompNS>
void RegistCallback::AddAssetCB()
{
	CompSerializer::Instance()->AddAssetFromJsonFunc(TComp::TYPE_NAME,
		[](const std::string& dir, const rapidjson::Value& val) -> n0::CompAssetPtr
    {
		auto comp = std::make_shared<TComp>();
		TCompNS seri;
		mm::LinearAllocator alloc;
		seri.LoadFromJson(alloc, dir, val);
		seri.StoreToMem(*comp);
		return comp;
	});

	CompSerializer::Instance()->AddAssetFromBinFunc(TComp::TYPE_NAME,
		[](const std::string& dir, bs::ImportStream& is) -> n0::CompAssetPtr
	{
		auto comp = std::make_shared<TComp>();
		TCompNS seri;
		seri.LoadFromBin(dir, is);
		seri.StoreToMem(*comp);
		return comp;
	});
}

}