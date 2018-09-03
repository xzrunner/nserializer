#include "ns/N0CompComplex.h"
#include "ns/NodeSerializer.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompSharedPatch.h>
#include <node2/AABBSystem.h>

namespace ns
{

size_t N0CompComplex::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;
	sz += sizeof(uint16_t);		// nodes num
	for (auto& node : m_nodes) {
		sz += NodeSerializer::GetBinSize(node, dir);
	}
	return sz;
}

void N0CompComplex::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	GD_ASSERT(m_nodes.size() < std::numeric_limits<uint16_t>::max(), "overflow");
	uint16_t sz = static_cast<uint16_t>(m_nodes.size());
	es.Write(sz);
	for (auto& node : m_nodes) {
		NodeSerializer::StoreToBin(node, dir, es);
	}
}

void N0CompComplex::LoadFromBin(const std::string& dir, bs::ImportStream& is)
{
	uint16_t sz = is.UInt16();
	for (size_t i = 0; i < sz; ++i)
	{
		auto node = std::make_shared<n0::SceneNode>();
		NodeSerializer::LoadFromBin(node, dir, is);
		AddNode(node);
	}
}

void N0CompComplex::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value nodes_val;
	nodes_val.SetArray();
	for (auto& node : m_nodes)
	{
		rapidjson::Value node_val;
		NodeSerializer::StoreToJson(node, dir, node_val, alloc);
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
		NodeSerializer::LoadFromJson(node, dir, *itr);
		AddNode(node);
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

void N0CompComplex::AddNode(const n0::SceneNodePtr& node)
{
	if (node->HasSharedComp<n0::CompAsset>())
	{
		auto& casset = node->GetSharedComp<n0::CompAsset>();
		auto rect = n2::AABBSystem::GetBounding(casset);
		node->AddUniqueComp<n2::CompBoundingBox>(rect);
	}

	if (node->HasUniqueComp<n2::CompSharedPatch>())
	{
		auto& cpatch = node->GetUniqueComp<n2::CompSharedPatch>();
		cpatch.PatchToNode(node);
	}

	m_nodes.push_back(node);
}

}