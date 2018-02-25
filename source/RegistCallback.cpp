#include "ns/RegistCallback.h"

#include "ns/CompColorCommon.h"
#include <node2/CompColorCommon.h>
#include "ns/CompColorMap.h"
#include <node2/CompColorMap.h>

#include "ns/CompTransform.h"
#include <node2/CompTransform.h>
#include "ns/CompBoundingBox.h"
#include <node2/CompBoundingBox.h>

#include "ns/CompImage.h"
#include <node2/CompImage.h>

#include "ns/CompNodeEditor.h"
#include <ee0/CompNodeEditor.h>

#include <node0/CompFactory.h>
#include <node0/CompStorer.h>
#include <node0/SceneNode.h>
#include <memmgr/LinearAllocator.h>

#define REGIST_CB(namespace, class)                                                    \
	n0::CompFactory::Instance()->AddCreator(namespace::class::TYPE_NAME, [](           \
		n0::SceneNodePtr& node, const std::string& dir, const rapidjson::Value& val) { \
			auto& comp = node->AddComponent<namespace::class>();                       \
			class ser;                                                                 \
			mm::LinearAllocator alloc;                                                 \
			ser.LoadFromJson(alloc, dir, val);                                         \
			ser.StoreToMem(comp);                                                      \
		}                                                                              \
	);                                                                                 \
	n0::CompStorer::Instance()->AddToJsonFunc(namespace::class::TYPE_NAME,             \
		[](const n0::NodeComponent& comp, const std::string& dir,                      \
           rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool       \
		{                                                                              \
			class seri;                                                                \
			seri.LoadFromMem(static_cast<const namespace::class&>(comp));              \
			seri.StoreToJson(dir, val, alloc);                                         \
			return true;                                                               \
		}                                                                              \
	);


namespace ns
{

void RegistCallback::Init()
{
	//n0::CompFactory::Instance()->AddCreator(n2::CompColorCommon::TYPE_NAME, 
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
	//n0::CompStorer::Instance()->AddToJsonFunc(n2::CompColorCommon::TYPE_NAME, 
	//	[](const n0::NodeComponent& comp, const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc)->bool
	//	{
	//		CompColorCommon seri;
	//		seri.LoadFromMem(static_cast<const n2::CompColorCommon&>(comp));
	//		seri.StoreToJson(dir, val, alloc);
	//		return true;
	//	}
	//);

	REGIST_CB(n2, CompColorCommon);
	REGIST_CB(n2, CompColorMap);

	REGIST_CB(n2, CompTransform);
	REGIST_CB(n2, CompBoundingBox);

	REGIST_CB(n2, CompImage);

	REGIST_CB(ee0, CompNodeEditor);
}

}