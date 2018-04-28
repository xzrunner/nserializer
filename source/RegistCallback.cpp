#include "ns/RegistCallback.h"
#include "ns/CompSerializer.h"
#include "ns/CompNoSerialize.h"

#include "ns/N2CompComplex.h"
#include <node2/CompComplex.h>
#include "ns/N2CompAnim.h"
#include <node2/CompAnim.h>
#include "ns/N2CompColorCommon.h"
#include <node2/CompColorCommon.h>
#include "ns/N2CompColorMap.h"
#include <node2/CompColorMap.h>

#include "ns/N2CompTransform.h"
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include "ns/N2CompUniquePatch.h"
#include <node2/CompUniquePatch.h>
#include "ns/N2CompSharedPatch.h"
#include <node2/CompSharedPatch.h>
#include "ns/N2CompScissor.h"
#include <node2/CompScissor.h>
#include "ns/N2CompScript.h"
#include <node2/CompScript.h>
#include "ns/N2CompScript.h"

// todo
#include <node2/EditOp.h>

#include "ns/N2CompImage.h"
#include <node2/CompImage.h>
#include "ns/N2CompMask.h"
#include <node2/CompMask.h>
#include "ns/N2CompText.h"
#include <node2/CompText.h>
#include "ns/N2CompScale9.h"
#include <node2/CompScale9.h>

#include "ns/N3CompAABB.h"
#include <node3/CompAABB.h>
#include "ns/N3CompTransform.h"
#include <node3/CompTransform.h>
#include "ns/N3CompModel.h"
#include <node3/CompModel.h>

#include "ns/EE0CompNodeEditor.h"
#include <ee0/CompNodeEditor.h>
#include "ns/EE0CompCustomProperties.h"
#include <ee0/CompCustomProperties.h>

#include <node0/SceneNode.h>
#include <memmgr/LinearAllocator.h>
#include <anim/Layer.h>
#include <anim/KeyFrame.h>

#define REGIST_UNIQUE_CB(ori, here)                                                    \
	CompSerializer::Instance()->AddFromJsonFunc(ori::TYPE_NAME,                        \
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)\
        {                                                                              \
			auto& comp = node->HasUniqueComp<ori>() ?                                  \
				node->GetUniqueComp<ori>() : node->AddUniqueComp<ori>();               \
			here ser;                                                                  \
			mm::LinearAllocator alloc;                                                 \
			ser.LoadFromJson(alloc, dir, val);                                         \
			ser.StoreToMem(comp);                                                      \
		}                                                                              \
	);                                                                                 \
	CompSerializer::Instance()->AddToJsonFunc(ori::TYPE_NAME,                          \
		[](const n0::NodeUniqueComp& comp, const std::string& dir,                     \
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool       \
		{                                                                              \
			here seri;                                                                 \
			seri.LoadFromMem(static_cast<const ori&>(comp));                           \
			seri.StoreToJson(dir, val, alloc);                                         \
			return true;                                                               \
		}                                                                              \
	);

#define REGIST_SHARED_CB(ori, here)                                                    \
	CompSerializer::Instance()->AddFromJsonFunc(ori::TYPE_NAME,                        \
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)\
        {                                                                              \
			auto& comp = node->HasSharedComp<ori>() ?                                  \
				node->GetSharedComp<ori>() : node->AddSharedComp<ori>();               \
			here ser;                                                                  \
			mm::LinearAllocator alloc;                                                 \
			ser.LoadFromJson(alloc, dir, val);                                         \
			ser.StoreToMem(comp);                                                      \
		}                                                                              \
	);                                                                                 \
	CompSerializer::Instance()->AddToJsonFunc(ori::TYPE_NAME,                          \
		[](const n0::NodeSharedComp& comp, const std::string& dir,                     \
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool       \
		{                                                                              \
			here seri;                                                                 \
			seri.LoadFromMem(static_cast<const ori&>(comp));                           \
			seri.StoreToJson(dir, val, alloc);                                         \
			return true;                                                               \
		}                                                                              \
	);

#define REGIST_UNIQUE_NULL_CB(ori, here)                                               \
	CompSerializer::Instance()->AddFromJsonFunc(ori::TYPE_NAME,                        \
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)\
        {                                                                              \
		}                                                                              \
	);                                                                                 \
	CompSerializer::Instance()->AddToJsonFunc(ori::TYPE_NAME,                          \
		[](const n0::NodeUniqueComp& comp, const std::string& dir,                     \
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool       \
		{                                                                              \
			return false;                                                              \
		}                                                                              \
	);

#define REGIST_ASSET_CB(ori, here)                                                     \
	CompSerializer::Instance()->AddAssetFromJsonFunc(ori::TYPE_NAME,                   \
		[](const std::string& dir, const rapidjson::Value& val) -> n0::CompAssetPtr    \
        {                                                                              \
			auto comp = std::make_shared<ori>();                                       \
			here ser;                                                                  \
			mm::LinearAllocator alloc;                                                 \
			ser.LoadFromJson(alloc, dir, val);                                         \
			ser.StoreToMem(*comp);                                                     \
			return comp;                                                               \
		}                                                                              \
	);                                                                                 \

namespace ns
{

void RegistCallback::Init()
{
	REGIST_UNIQUE_CB(n2::CompColorCommon, N2CompColorCommon);
	REGIST_UNIQUE_CB(n2::CompColorMap, N2CompColorMap);

	REGIST_UNIQUE_CB(n2::CompTransform, N2CompTransform);
	REGIST_UNIQUE_NULL_CB(n2::CompBoundingBox, CompNoSerialize);
	REGIST_UNIQUE_CB(n2::CompUniquePatch, N2CompUniquePatch);
	REGIST_UNIQUE_CB(n2::CompSharedPatch, N2CompSharedPatch);
	REGIST_UNIQUE_CB(n2::CompScissor, N2CompScissor);
	REGIST_UNIQUE_CB(n2::CompScript, N2CompScript);

	REGIST_SHARED_CB(n2::CompComplex, N2CompComplex);
	REGIST_SHARED_CB(n2::CompAnim, N2CompAnim);
	REGIST_SHARED_CB(n2::CompImage, N2CompImage);
	REGIST_SHARED_CB(n2::CompMask, N2CompMask);
	REGIST_SHARED_CB(n2::CompText, N2CompText);
	REGIST_SHARED_CB(n2::CompScale9, N2CompScale9);

	REGIST_ASSET_CB(n2::CompComplex, N2CompComplex);
	REGIST_ASSET_CB(n2::CompAnim, N2CompAnim);
	REGIST_ASSET_CB(n2::CompImage, N2CompImage);
	REGIST_ASSET_CB(n2::CompMask, N2CompMask);
	REGIST_ASSET_CB(n2::CompText, N2CompText);
	REGIST_ASSET_CB(n2::CompScale9, N2CompScale9);

	REGIST_UNIQUE_CB(n3::CompAABB, N3CompAABB);
	REGIST_UNIQUE_CB(n3::CompTransform, N3CompTransform);
	REGIST_SHARED_CB(n3::CompModel, N3CompModel);

	REGIST_UNIQUE_CB(ee0::CompNodeEditor, EE0CompNodeEditor);
	REGIST_UNIQUE_CB(ee0::CompCustomProperties, EE0CompCustomProperties);
}

}