#include "ns/NodeFactory.h"
#include "ns/NodeSerializer.h"
#include "ns/Blackboard.h"
#include "ns/CompFactory.h"

#include <sx/ResFileHelper.h>
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompImage.h>
#include <node2/AABBSystem.h>
#include <node3/CompModel.h>
#include <node3/CompModelInst.h>
#include <node3/CompTransform.h>
#include <node3/CompAABB.h>
#include <model/Model.h>
#include <facade/ResPool.h>
#include <facade/Image.h>
#include <facade/Texture.h>

#include <string>

namespace ns
{

n0::SceneNodePtr NodeFactory::Create(const std::string& dir, const rapidjson::Value& val)
{
	auto node = std::make_shared<n0::SceneNode>();
	ns::NodeSerializer::LoadFromJson(node, dir, val);
	return node;
}

n0::SceneNodePtr NodeFactory::Create(const std::string& filepath)
{
	n0::SceneNodePtr node = nullptr;
	auto type = sx::ResFileHelper::Type(filepath);
	switch (type)
	{
	case sx::RES_FILE_BIN:
	case sx::RES_FILE_JSON:
		node = CreateFromCommon(filepath);
		break;
	case sx::RES_FILE_IMAGE:
		node = CreateFromImage(filepath);
		break;
	case sx::RES_FILE_MODEL:
		node = CreateFromModel(filepath);
		break;
	}
	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromCommon(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// asset
	auto casset = CompFactory::Instance()->CreateAsset(filepath);
	// fixme: node already has asset
	node->AddSharedCompNoCreate<n0::CompAsset>(casset);

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	auto aabb = n2::AABBSystem::GetBounding(*casset);
	node->AddUniqueComp<n2::CompBoundingBox>(aabb);

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromImage(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// image
	auto img = facade::ResPool::Instance().Fetch<facade::Image>(filepath);
	auto& cimage = node->AddSharedComp<n2::CompImage>();
	cimage.SetFilepath(filepath);
	cimage.SetTexture(img->GetTexture());

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	sm::rect sz(img->GetWidth(), img->GetHeight());
	node->AddUniqueComp<n2::CompBoundingBox>(sz);

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromModel(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// model
	auto& cmodel = node->AddSharedComp<n3::CompModel>();
	auto model = facade::ResPool::Instance().Fetch<model::Model>(filepath);
	cmodel.SetModel(model);
	cmodel.SetFilepath(filepath);
	node->AddUniqueComp<n3::CompModelInst>(model, 0);

	// transform
	auto& ctrans = node->AddUniqueComp<n3::CompTransform>();

	// aabb
	node->AddUniqueComp<n3::CompAABB>(pt3::AABB(model->aabb));

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	return node;
}

void NodeFactory::InitCompId(n0::CompIdentity& cid, const std::string& filepath)
{
	cid.SetFilepath(filepath);
	auto id = Blackboard::Instance()->GenNodeId();
	cid.SetID(id);
	cid.SetName("_obj" + std::to_string(id));
}

}