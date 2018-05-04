#include "ns/NodeFactory.h"
#include "ns/NodeSerializer.h"
#include "ns/CompSerializer.h"
#include "ns/Blackboard.h"

#include <ee0/CompNodeEditor.h>

#include <sx/ResFileHelper.h>
#include <js/RapidJsonHelper.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompImage.h>
#include <node2/AABBSystem.h>
#include <node3/CompModel.h>
#include <node3/CompTransform.h>
#include <node3/CompAABB.h>
#include <model/Model.h>
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
	case sx::FILE_IMAGE:
		node = CreateFromImage(filepath);
		break;
	case sx::FILE_JSON:
		node = CreateFromJson(filepath);
		break;
	case sx::FILE_MODEL:
		node = CreateFromModel(filepath);
		break;
	}
	return node;
}

void NodeFactory::CreateNodeAssetComp(n0::SceneNodePtr& node, const std::string& filepath)
{
	auto casset = facade::ResPool::Instance().Query<n0::CompAsset>(filepath);
	if (casset)
	{
		node->AddSharedCompNoCreate<n0::CompAsset>(casset);
	}
	else
	{
		rapidjson::Document doc;
		js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

		auto dir = boost::filesystem::path(filepath).parent_path().string();
		ns::CompSerializer::Instance()->FromJson(node, dir, doc);

		auto casset = node->GetSharedCompPtr<n0::CompAsset>();
		bool succ = facade::ResPool::Instance().Insert<n0::CompAsset>(filepath, casset);
		GD_ASSERT(succ, "exists");
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
	case sx::FILE_IMAGE:
		{
			auto img = facade::ResPool::Instance().Fetch<facade::Image>(filepath);
			auto cimage = std::make_shared<n2::CompImage>();
			cimage->SetFilepath(filepath);
			cimage->SetTexture(img->GetTexture());
			casset = cimage;
		}
		break;
	case sx::FILE_JSON:
		{
			rapidjson::Document doc;
			js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

			auto dir = boost::filesystem::path(filepath).parent_path().string();
			casset = ns::CompSerializer::Instance()->AssetFromJson(dir, doc);
			facade::ResPool::Instance().Insert<n0::CompAsset>(filepath, casset);
		}
		break;
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
	InitCompEditor(ceditor, filepath);

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

	// editor
	auto& ceditor = node->AddUniqueComp<ee0::CompNodeEditor>();
	InitCompEditor(ceditor, filepath);

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

	// transform
	auto& ctrans = node->AddUniqueComp<n3::CompTransform>();

	// aabb
	auto& aabb = model->GetAABB();
	auto& caabb = node->AddUniqueComp<n3::CompAABB>(pt3::AABB(aabb));

	// editor
	auto& ceditor = node->AddUniqueComp<ee0::CompNodeEditor>();
	InitCompEditor(ceditor, filepath);

	return node;
}

void NodeFactory::InitCompEditor(ee0::CompNodeEditor& ceditor, const std::string& filepath)
{
	ceditor.SetFilepath(filepath);
	auto id = Blackboard::Instance()->GenNodeId();
	ceditor.SetID(id);
	ceditor.SetName("_obj" + std::to_string(id));
}

}