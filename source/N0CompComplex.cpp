#include "ns/N0CompComplex.h"
#include "ns/NodeSerializer.h"

#include <node0/CompComplex.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>

namespace ns
{

size_t N0CompComplex::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N0CompComplex::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N0CompComplex::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N0CompComplex::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value nodes_val;
	nodes_val.SetArray();
	for (auto& node : m_nodes) 
	{
		rapidjson::Value node_val;
		NodeSerializer::StoreNodeToJson(node, dir, node_val, alloc);
		nodes_val.PushBack(node_val, alloc);
	}

	val.AddMember("nodes", nodes_val, alloc);
}

void N0CompComplex::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto& nodes_val = val["nodes"];
	for (auto itr = nodes_val.Begin(); itr != nodes_val.End(); ++itr)
	{
		auto node = std::make_shared<n0::SceneNode>();
		NodeSerializer::LoadNodeFromJson(node, dir, *itr);

		if (node->HasComponent<n2::CompBoundingBox>() &&
			node->HasComponent<n2::CompTransform>())
		{
			auto& cbb = node->GetComponent<n2::CompBoundingBox>();
			auto& ctrans = node->GetComponent<n2::CompTransform>();
			cbb.Build(ctrans.GetTrans().GetSRT());
		}

		m_nodes.push_back(node);
	}
}

void N0CompComplex::StoreToMem(n0::CompComplex& comp) const
{
	comp.SetChildren(m_nodes);
}

void N0CompComplex::LoadFromMem(const n0::CompComplex& comp)
{
	m_nodes = comp.GetAllChildren();
}

}