#pragma once

#include "ns/CompSerializer.h"
#include "ns/CompFactory.h"

#include <node0/SceneNode.h>
#include <memmgr/LinearAllocator.h>

namespace ns
{

template <typename TComp, typename TCompNS>
void RegistCallback::AddUniqueCB()
{
	CompFactory::Instance()->AddCreateFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node)->n0::NodeComp&
	{
		return node->HasUniqueComp<TComp>() ?
			node->GetUniqueComp<TComp>() : node->AddUniqueComp<TComp>();
	});

	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::NodeComp& comp, const std::string& dir, const rapidjson::Value& val)
	{
		TCompNS seri;
		mm::LinearAllocator alloc;
		seri.LoadFromJson(alloc, dir, val);
		seri.StoreToMem(static_cast<TComp&>(comp));
	});
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir,
		   rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		seri.StoreToJson(dir, val, alloc);
		return true;
	});

	CompSerializer::Instance()->AddGetBinSizeFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir)->size_t
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		return seri.GetBinSize(dir);
	});
	CompSerializer::Instance()->AddFromBinFunc(TComp::TYPE_NAME,
		[](n0::NodeComp& comp, const std::string& dir, bs::ImportStream& is)
	{
		TCompNS seri;
		seri.LoadFromBin(dir, is);
		seri.StoreToMem(static_cast<TComp&>(comp));
	});
	CompSerializer::Instance()->AddToBinFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir, bs::ExportStream& es)->bool
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
	CompFactory::Instance()->AddCreateFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node)->n0::NodeComp&
	{
		return node->HasSharedComp<TComp>() ?
			node->GetSharedComp<TComp>() : node->AddSharedComp<TComp>();
	});

	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::NodeComp& comp, const std::string& dir, const rapidjson::Value& val)
	{
		TCompNS seri;
		mm::LinearAllocator alloc;
		seri.LoadFromJson(alloc, dir, val);
		seri.StoreToMem(static_cast<TComp&>(comp));
	});
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir,
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		seri.StoreToJson(dir, val, alloc);
		return true;
	});

	CompSerializer::Instance()->AddGetBinSizeFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir)->size_t
	{
		TCompNS seri;
		seri.LoadFromMem(static_cast<const TComp&>(comp));
		return seri.GetBinSize(dir);
	});
	CompSerializer::Instance()->AddFromBinFunc(TComp::TYPE_NAME,
		[](n0::NodeComp& comp, const std::string& dir, bs::ImportStream& is)
	{
		TCompNS seri;
		seri.LoadFromBin(dir, is);
		seri.StoreToMem(static_cast<TComp&>(comp));
	});
	CompSerializer::Instance()->AddToBinFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir, bs::ExportStream& es)->bool
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
	CompFactory::Instance()->AddCreateFunc(TComp::TYPE_NAME,
		[](n0::SceneNodePtr& node)->n0::NodeComp&
	{
		return node->HasUniqueComp<TComp>() ?
			node->GetUniqueComp<TComp>() : node->AddUniqueComp<TComp>();
	});

	CompSerializer::Instance()->AddFromJsonFunc(TComp::TYPE_NAME,
		[](n0::NodeComp& comp, const std::string& dir, const rapidjson::Value& val)
    {
	});
	CompSerializer::Instance()->AddToJsonFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir,
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	{
		return false;
	});

	CompSerializer::Instance()->AddGetBinSizeFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir)->size_t
	{
		return 0;
	});
	CompSerializer::Instance()->AddFromBinFunc(TComp::TYPE_NAME,
		[](n0::NodeComp& comp, const std::string& dir, bs::ImportStream& is)
    {
	});
	CompSerializer::Instance()->AddToBinFunc(TComp::TYPE_NAME,
		[](const n0::NodeComp& comp, const std::string& dir, bs::ExportStream& es)->bool
	{
		return false;
	});
}

template <typename TComp>
void RegistCallback::AddAssetCB()
{
	CompFactory::Instance()->AddCreateAssetFunc(TComp::TYPE_NAME,
		[]()->n0::CompAssetPtr
	{
		return std::make_shared<TComp>();
	});
}

}