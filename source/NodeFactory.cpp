#include "ns/NodeFactory.h"
#include "ns/NodeSerializer.h"
#include "ns/Blackboard.h"
#include "ns/CompFactory.h"

#include <unirender/Texture.h>
#include <unirender/Device.h>
#include <sx/ResFileHelper.h>
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompImage.h>
#include <node2/AABBSystem.h>
#include <node2/CompScript.h>
#include <node3/CompModel.h>
#include <node3/CompModelInst.h>
#include <node3/CompTransform.h>
#include <node3/CompAABB.h>
#include <node3/CompImage3D.h>
#include <model/Model.h>
#include <facade/ResPool.h>
#include <facade/Image.h>
#include <facade/Image3D.h>

#include <string>

namespace ns
{

n0::SceneNodePtr NodeFactory::Create(const ur::Device& dev, const std::string& filepath)
{
	n0::SceneNodePtr node = nullptr;
	auto type = sx::ResFileHelper::Type(filepath);
	switch (type)
	{
	case sx::RES_FILE_BIN:
	case sx::RES_FILE_JSON:
		node = CreateFromCommon(dev, filepath);
		break;
	case sx::RES_FILE_IMAGE:
		node = CreateFromImage(dev, filepath);
		break;
	case sx::RES_FILE_IMAGE3D:
		node = CreateFromImage3D(dev, filepath);
		break;
	case sx::RES_FILE_MODEL:
		node = CreateFromModel(dev, filepath);
		break;
	case sx::RES_FILE_MAP:
		node = CreateFromModel(dev, filepath);
		break;
	case sx::RES_FILE_SCRIPT:
		node = CreateFromScript(filepath);
		break;
	}
	return node;
}

n0::SceneNodePtr NodeFactory::Create2D()
{
	auto node = std::make_shared<n0::SceneNode>();

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	auto aabb = n2::AABBSystem::Instance()->GetBounding(*node);
	node->AddUniqueComp<n2::CompBoundingBox>(aabb);

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, "");

	return node;
}

n0::SceneNodePtr NodeFactory::Create3D()
{
	auto node = std::make_shared<n0::SceneNode>();

	// transform
	node->AddUniqueComp<n3::CompTransform>();

	// aabb
	node->AddUniqueComp<n3::CompAABB>();

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, "");

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromCommon(const ur::Device& dev, const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// asset
	auto casset = CompFactory::Instance()->CreateAsset(dev, filepath);
	if (node->HasSharedComp<n0::CompAsset>()) {
		node->RemoveSharedComp<n0::CompAsset>();
	}
	node->AddSharedCompNoCreate<n0::CompAsset>(casset);

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	auto aabb = n2::AABBSystem::Instance()->GetBounding(*node);
	node->AddUniqueComp<n2::CompBoundingBox>(aabb);

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromImage(const ur::Device& dev, const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// image
	auto img = facade::ResPool::Instance().Fetch<facade::Image>(filepath, &dev);
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

n0::SceneNodePtr NodeFactory::CreateFromImage3D(const ur::Device& dev, const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// image
	auto img = facade::ResPool::Instance().Fetch<facade::Image3D>(filepath, &dev);
	auto& cimage = node->AddSharedComp<n3::CompImage3D>();
	cimage.SetFilepath(filepath);
	cimage.SetTexture(img->GetTexture());

	// transform
	auto& ctrans = node->AddUniqueComp<n3::CompTransform>();

	// aabb
	auto& tex = cimage.GetTexture();
	float w = static_cast<float>(tex->GetWidth());
	float h = static_cast<float>(tex->GetHeight());
	float d = static_cast<float>(tex->GetDepth());
	h /= w;
	d /= w;
	w = 1.0f;
	node->AddUniqueComp<n3::CompAABB>(pt3::AABB(sm::cube(w, h, d)));

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromModel(const ur::Device& dev, const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// model
	auto& cmodel = node->AddSharedComp<n3::CompModel>();
	auto model = facade::ResPool::Instance().Fetch<model::Model>(filepath, &dev);
	cmodel.SetModel(model);cmodel.SetFilepath(filepath);
	node->AddUniqueComp<n3::CompModelInst>(model, 0);

	// transformauto& ctrans = node->AddUniqueComp<n3::CompTransform>();

	// aabb
	node->AddUniqueComp<n3::CompAABB>(pt3::AABB(model->aabb));

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromScript(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// script
	auto& cscript = node->AddUniqueComp<n2::CompScript>();
	cscript.SetFilepath(filepath);
	cscript.Reload(node);

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	auto aabb = n2::AABBSystem::Instance()->GetBounding(*node);
	node->AddUniqueComp<n2::CompBoundingBox>(aabb);

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	//
	cscript.GetImpl().Init();

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