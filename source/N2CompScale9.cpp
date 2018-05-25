#include "ns/N2CompScale9.h"
#include "ns/CompType.h"
#include "ns/NodeFactory.h"
#include "ns/NodeSerializer.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/SceneNode.h>
#include <node2/CompScale9.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>

#include <boost/filesystem.hpp>

namespace ns
{

N2CompScale9::N2CompScale9()
	: m_type(n2::CompScale9::S9_NULL)
	, m_width(0)
	, m_height(0)
{
}

size_t N2CompScale9::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);		// type

	sz += sizeof(uint8_t);		// grid num
	for (auto& grid : m_grids) {
		sz += NodeSerializer::GetBinSize(grid, dir);
	}

	sz += sizeof(uint16_t);		// width
	sz += sizeof(uint16_t);		// height

	return sz;
}

void N2CompScale9::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	es.Write(static_cast<uint8_t>(m_type));

	es.Write(static_cast<uint8_t>(m_grids.size()));
	for (auto& grid : m_grids) {
		NodeSerializer::StoreToBin(grid, dir, es);
	}

	es.Write(static_cast<uint16_t>(m_width));
	es.Write(static_cast<uint16_t>(m_height));
}

void N2CompScale9::LoadFromBin(const std::string& dir, bs::ImportStream& is)
{
	m_type = is.UInt8();

	size_t num = is.UInt16();
	m_grids.resize(num);
	for (size_t i = 0; i < num; ++i)
	{
		auto node = std::make_shared<n0::SceneNode>();
		NodeSerializer::LoadFromBin(node, dir, is);
		m_grids[i] = node;
	}

	m_width = is.UInt16();
	m_height = is.UInt16();
}

void N2CompScale9::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("s9_type", m_type, alloc);

	rapidjson::Value grids_val;
	grids_val.SetArray();
	for (auto& node : m_grids)
	{
		rapidjson::Value node_val;
		NodeSerializer::StoreToJson(node, dir, node_val, alloc);
		grids_val.PushBack(node_val, alloc);
	}
	val.AddMember("grids", grids_val, alloc);

	val.AddMember("width", m_width, alloc);
	val.AddMember("height", m_height, alloc);
}

void N2CompScale9::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	m_type = val["s9_type"].GetInt();

	auto& grids_val = val["grids"];
	for (auto itr = grids_val.Begin(); itr != grids_val.End(); ++itr)
	{
		auto node = std::make_shared<n0::SceneNode>();
		NodeSerializer::LoadFromJson(node, dir, *itr);
		m_grids.push_back(node);
	}

	m_width = val["width"].GetFloat();
	m_height = val["height"].GetFloat();
}

void N2CompScale9::StoreToMem(n2::CompScale9& comp) const
{
	n0::SceneNodePtr grids[9];

	auto itr = m_grids.begin();
	switch (m_type)
	{
	case n2::CompScale9::S9_9GRID:
		for (int i = 0; i < 9; ++i) {
			grids[i] = *itr++;
		}
		break;
	case n2::CompScale9::S9_3GRID_HORI:
		grids[n2::CompScale9::S9_MID_LEFT]   = m_grids[0];
		grids[n2::CompScale9::S9_MID_CENTER] = m_grids[1];
		grids[n2::CompScale9::S9_MID_RIGHT]  = m_grids[2];
		break;
	case n2::CompScale9::S9_3GRID_VERT:
		grids[n2::CompScale9::S9_DOWN_CENTER] = m_grids[0];
		grids[n2::CompScale9::S9_MID_CENTER]  = m_grids[1];
		grids[n2::CompScale9::S9_TOP_CENTER]  = m_grids[2];
		break;
	case n2::CompScale9::S9_6GRID_UPPER:
		grids[n2::CompScale9::S9_MID_LEFT]   = m_grids[0];
		grids[n2::CompScale9::S9_MID_CENTER] = m_grids[1];
		grids[n2::CompScale9::S9_MID_RIGHT]  = m_grids[2];
		grids[n2::CompScale9::S9_TOP_LEFT]   = m_grids[3];
		grids[n2::CompScale9::S9_TOP_CENTER] = m_grids[4];
		grids[n2::CompScale9::S9_TOP_RIGHT]  = m_grids[5];
		break;
	case n2::CompScale9::S9_9GRID_HOLLOW:
		break;
	}

	comp.Build(static_cast<n2::CompScale9::Scale9Type>(m_type),
		m_width, m_height, grids, 0, 0, 0, 0);
	comp.SetSize(m_width, m_height);
}

void N2CompScale9::LoadFromMem(const n2::CompScale9& comp)
{
	m_type = comp.GetType();

	comp.Traverse([&](const n0::SceneNodePtr& node)->bool {
		m_grids.push_back(node);
		return true;
	});

	m_width = comp.GetWidth();
	m_height = comp.GetHeight();
}

}