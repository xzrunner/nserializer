#include "ns/NodeFactory.h"
#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"
#include "ns/Blackboard.h"

#include <ee0/CompNodeEditor.h>

#include <sx/ResFileHelper.h>
#include <js/RapidJsonHelper.h>
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
#include <model/ModelInstance.h>
#include <facade/ResPool.h>
#include <facade/Image.h>
#include <facade/Texture.h>

#include <boost/filesystem.hpp>

#include <string>

namespace ns
{

n0::SceneNodePtr NodeFactory::Create(const std::string& dir, const rapidjson::Value& val)
{
	auto node = std::make_shared<n0::SceneNode>();
	ns::NodeSerializer::LoadNodeFromJson(node, dir, val);
	return node;
}

n0::SceneNodePtr NodeFactory::Create(const std::string& filepath)
{
	n0::SceneNodePtr node = nullptr;
	auto type = sx::ResFileHelper::Type(filepath);
	switch (type)
	{
	case sx::RES_FILE_IMAGE:
		node = CreateFromImage(filepath);
		break;
	case sx::RES_FILE_JSON:
		node = CreateFromJson(filepath);
		break;
	case sx::RES_FILE_MODEL:
		node = CreateFromModel(filepath);
		break;
	}
	return node;
}

void NodeFactory::CreateNodeAssetComp(n0::SceneNodePtr& node, const std::string& filepath)
{
	if (auto casset = facade::ResPool::Instance().Query<n0::CompAsset>(filepath))
	{
		// fixme: node already has asset
		node->AddSharedCompNoCreate<n0::CompAsset>(casset);
	}
	else
	{
		auto type = sx::ResFileHelper::Type(filepath);
		switch (type)
		{
		case sx::RES_FILE_JSON:
			{
				rapidjson::Document doc;
				js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

				auto dir = boost::filesystem::path(filepath).parent_path().string();
				ns::CompSerializer::Instance()->FromJson(node, dir, doc);

				auto casset = node->GetSharedCompPtr<n0::CompAsset>();
				bool succ = facade::ResPool::Instance().Insert<n0::CompAsset>(filepath, casset);
				GD_ASSERT(succ, "exists");
			}
			break;
		case sx::RES_FILE_MODEL:
			{
				auto model = facade::ResPool::Instance().Fetch<model::Model>(filepath);
				if (!node->HasSharedComp<n3::CompModel>()) {
					node->AddSharedComp<n3::CompModel>();
				}
				auto cmodel = node->GetSharedCompPtr<n3::CompModel>();
				cmodel->SetFilepath(filepath);
				cmodel->SetModel(model);
				if (node->HasUniqueComp<n3::CompModelInst>()) {
					auto& inst = node->GetUniqueComp<n3::CompModelInst>();
					inst.SetModel(model, 0);
				} else {
					node->AddUniqueComp<n3::CompModelInst>(model, 0);
				}
				bool succ = facade::ResPool::Instance().Insert<n0::CompAsset>(filepath, cmodel);
				GD_ASSERT(succ, "exists");
			}
			break;
		}
	}
}

n0::CompAssetPtr NodeFactory::CreateAssetComp(const std::string& filepath)
{
	auto casset = facade::ResPool::Instance().Query<n0::CompAsset>(filepath);
	if (casset) {
		return casset;
	}

	auto type = sx::ResFileHelper::Type(filepath);
	switch (type)
	{
	case sx::RES_FILE_IMAGE:
		{
			auto img = facade::ResPool::Instance().Fetch<facade::Image>(filepath);
			auto cimage = std::make_shared<n2::CompImage>();
			cimage->SetFilepath(filepath);
			cimage->SetTexture(img->GetTexture());
			casset = cimage;
		}
		break;
	case sx::RES_FILE_JSON:
		{
			rapidjson::Document doc;
			js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

			auto dir = boost::filesystem::path(filepath).parent_path().string();
			casset = ns::CompSerializer::Instance()->AssetFromJson(dir, doc);
		}
		break;
	case sx::RES_FILE_MODEL:
		{
			auto model = facade::ResPool::Instance().Fetch<model::Model>(filepath);
			auto cmodel = std::make_shared<n3::CompModel>();
			cmodel->SetFilepath(filepath);
			cmodel->SetModel(model);
			casset = cmodel;
		}
		break;
	}

	if (casset) {
		facade::ResPool::Instance().Insert<n0::CompAsset>(filepath, casset);
	}

	return casset;
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

	// editor
	auto& ceditor = node->AddUniqueComp<ee0::CompNodeEditor>();

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromJson(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// asset
	CreateNodeAssetComp(node, filepath);
	auto& casset = node->GetSharedComp<n0::CompAsset>();

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	auto aabb = n2::AABBSystem::GetBounding(casset);
	node->AddUniqueComp<n2::CompBoundingBox>(aabb);

	// id
	auto& cid = node->AddUniqueComp<n0::CompIdentity>();
	InitCompId(cid, filepath);

	// editor
	auto& ceditor = node->AddUniqueComp<ee0::CompNodeEditor>();

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

	// editor
	auto& ceditor = node->AddUniqueComp<ee0::CompNodeEditor>();

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