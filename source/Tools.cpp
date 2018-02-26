#include "ns/Tools.h"

#include <js/RapidJsonHelper.h>
#include <node0/SerializeSystem.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>

#include <boost/filesystem.hpp>

#include <string>

namespace ns
{

n0::SceneNodePtr Tools::CreateNode(const std::string& dir, const rapidjson::Value& val)
{
	auto node = std::make_shared<n0::SceneNode>();
	n0::SerializeSystem::LoadNodeFromJson(node, dir, val);

	if (node->HasComponent<n2::CompBoundingBox>() &&
		node->HasComponent<n2::CompTransform>())
	{
		auto& cbb = node->GetComponent<n2::CompBoundingBox>();
		auto& ctrans = node->GetComponent<n2::CompTransform>();
		cbb.Build(ctrans.GetTrans().GetSRT());
	}

	return node;
}

n0::SceneNodePtr Tools::CreateNode(const std::string& filepath)
{
	rapidjson::Document doc;
	js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

	auto& nodes_val = doc["nodes"];
	if (nodes_val.Size() == 1) {
		auto dir = boost::filesystem::path(filepath).parent_path().string();
		return CreateNode(dir, nodes_val[0]);
	} else {
		return nullptr;
	}

}

}