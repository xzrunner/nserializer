#include "ns/RegistCallback.h"
#include "ns/CompSerializer.h"

#include "ns/N0CompComplex.h"
#include <node0/CompComplex.h>

#include "ns/N2CompColorCommon.h"
#include <node2/CompColorCommon.h>
#include "ns/N2CompColorMap.h"
#include <node2/CompColorMap.h>

#include "ns/N2CompTransform.h"
#include <node2/CompTransform.h>
#include "ns/N2CompBoundingBox.h"
#include <node2/CompBoundingBox.h>

#include "ns/N2CompImage.h"
#include <node2/CompImage.h>
#include "ns/N2CompMask.h"
#include <node2/CompMask.h>
#include "ns/N2CompSprite2.h"
#include <node2/CompSprite2.h>
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

#include <node0/SceneNode.h>
#include <memmgr/LinearAllocator.h>

#define REGIST_CB(ori, here)                                                           \
	CompSerializer::Instance()->AddFromJsonFunc(ori::TYPE_NAME,                        \
		[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val)\
        {                                                                              \
			auto& comp = node->HasComponent<ori>() ?                                   \
				node->GetComponent<ori>() : node->AddComponent<ori>();                 \
			here ser;                                                                  \
			mm::LinearAllocator alloc;                                                 \
			ser.LoadFromJson(alloc, dir, val);                                         \
			ser.StoreToMem(comp);                                                      \
		}                                                                              \
	);                                                                                 \
	CompSerializer::Instance()->AddToJsonFunc(ori::TYPE_NAME,                          \
		[](const n0::NodeComponent& comp, const std::string& dir,                      \
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool       \
		{                                                                              \
			here seri;                                                                 \
			seri.LoadFromMem(static_cast<const ori&>(comp));                           \
			seri.StoreToJson(dir, val, alloc);                                         \
			return true;                                                               \
		}                                                                              \
	);


namespace ns
{

void RegistCallback::Init()
{
	//CompSerializer::Instance()->AddCreator(n2::CompColorCommon::TYPE_NAME, 
	//	[](n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val) 
	//	{
	//		auto& comp = node->AddComponent<n2::CompColorCommon>();
	//		CompColorCommon seri;
	//		mm::LinearAllocator alloc;
	//		seri.LoadFromJson(alloc, dir, val);
	//		seri.StoreToMem(comp);
	//	}
	//);
	//
	//CompSerializer::Instance()->AddToJsonFunc(n2::CompColorCommon::TYPE_NAME, 
	//	[](const n0::NodeComponent& comp, const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	//	{
	//		CompColorCommon seri;
	//		seri.LoadFromMem(static_cast<const n2::CompColorCommon&>(comp));
	//		seri.StoreToJson(dir, val, alloc);
	//		return true;
	//	}
	//);

	REGIST_CB(n0::CompComplex, N0CompComplex);

	REGIST_CB(n2::CompColorCommon, N2CompColorCommon);
	REGIST_CB(n2::CompColorMap, N2CompColorMap);

	REGIST_CB(n2::CompTransform, N2CompTransform);
	REGIST_CB(n2::CompBoundingBox, N2CompBoundingBox);

	REGIST_CB(n2::CompImage, N2CompImage);
	REGIST_CB(n2::CompMask, N2CompMask);
	REGIST_CB(n2::CompSprite2, N2CompSprite2);
	REGIST_CB(n2::CompText, N2CompText);
	REGIST_CB(n2::CompScale9, N2CompScale9);

	REGIST_CB(n3::CompAABB, N3CompAABB);
	REGIST_CB(n3::CompTransform, N3CompTransform);
	REGIST_CB(n3::CompModel, N3CompModel);

	REGIST_CB(ee0::CompNodeEditor, EE0CompNodeEditor);
}

}