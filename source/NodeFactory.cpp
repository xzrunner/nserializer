#include "ns/NodeFactory.h"

#include <ee0/CompNodeEditor.h>

#include <js/RapidJsonHelper.h>
#include <ns/NodeSerializer.h>
#include <ns/CompSerializer.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompImage.h>
#include <gum/ResPool.h>
#include <gum/Image.h>
#include <gum/Texture.h>
#include <gum/ResPool.h>

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

	std::string ext = filepath.substr(filepath.rfind('.') + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "ppm" || ext == "pvr" || ext == "pkm") {
		node = CreateFromImage(filepath);
	} else if (ext == "json") {
		node = CreateFromJson(filepath);
	}

	return node;
}

void NodeFactory::CreateNodeAssetComp(n0::SceneNodePtr& node, const std::string& filepath)
{
	auto casset = gum::ResPool::Instance().Query<n0::CompAsset>(filepath);
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
		bool succ = gum::ResPool::Instance().Insert<n0::CompAsset>(filepath, casset);
		GD_ASSERT(succ, "exists");
	}
}

n0::SceneNodePtr NodeFactory::CreateFromImage(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// image
	auto img = gum::ResPool::Instance().Fetch<gum::Image>(filepath);
	auto& cimage = node->AddSharedComp<n2::CompImage>();
	cimage.SetFilepath(filepath);
	cimage.SetTexture(img->GetTexture());

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();

	// aabb
	sm::rect sz(img->GetWidth(), img->GetHeight());
	auto& cbounding = node->AddUniqueComp<n2::CompBoundingBox>(sz);
	cbounding.Build(ctrans.GetTrans().GetSRT());

	// editor
	node->AddUniqueComp<ee0::CompNodeEditor>();

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
	auto& cbounding = node->AddUniqueComp<n2::CompBoundingBox>(casset.GetBounding());
	cbounding.Build(ctrans.GetTrans().GetSRT());

	// editor
	auto& ceditor = node->AddUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);

	return node;
}

}