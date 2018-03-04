#include "ns/NodeFactory.h"

#include <ee0/CompNodeEditor.h>

#include <js/RapidJsonHelper.h>
#include <ns/NodeSerializer.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompImage.h>
#include <gum/ResPool.h>
#include <gum/Image.h>
#include <gum/Texture.h>

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

n0::SceneNodePtr NodeFactory::CreateFromImage(const std::string& filepath)
{
	auto node = std::make_shared<n0::SceneNode>();

	// image
	auto img = gum::ResPool::Instance().Fetch<gum::Image>(filepath);
	auto& cimage = node->AddComponent<n2::CompImage>();
	cimage.SetFilepath(filepath);
	cimage.SetTexture(img->GetTexture());

	// transform
	auto& ctrans = node->AddComponent<n2::CompTransform>();

	// aabb
	sm::rect sz(img->GetWidth(), img->GetHeight());
	auto& cbounding = node->AddComponent<n2::CompBoundingBox>(sz);
	cbounding.Build(ctrans.GetTrans().GetSRT());

	// editor
	node->AddComponent<ee0::CompNodeEditor>();

	return node;
}

n0::SceneNodePtr NodeFactory::CreateFromJson(const std::string& filepath)
{
	rapidjson::Document doc;
	js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

	auto& nodes_val = doc["nodes"];
	if (nodes_val.Size() == 1) {
		auto dir = boost::filesystem::path(filepath).parent_path().string();
		return Create(dir, nodes_val[0]);
	} else {
		return nullptr;
	}
}

}