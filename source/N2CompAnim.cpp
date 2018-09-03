#include "ns/N2CompAnim.h"
#include "ns/NodeSerializer.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/SceneNode.h>
#include <node2/CompAnim.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompSharedPatch.h>
#include <node2/AABBSystem.h>
#include <anim/Layer.h>
#include <anim/KeyFrame.h>

namespace ns
{

size_t N2CompAnim::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;
	sz += sizeof(uint16_t);		// layer num
	for (auto& layer : m_layers)
	{
		sz += sizeof(uint16_t);		// frame num
		for (auto& frame : layer->GetAllKeyFrames())
		{
			sz += sizeof(uint16_t);		// frame idx
			sz += sizeof(uint8_t);		// tween

			sz += sizeof(uint16_t);		// node num
			for (auto& node : frame->GetAllNodes()) {
				sz += NodeSerializer::GetBinSize(node, dir); // node
			}
		}
	}
	return sz;
}

void N2CompAnim::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	GD_ASSERT(m_layers.size() < std::numeric_limits<uint16_t>::max(), "overflow");
	es.Write(static_cast<uint16_t>(m_layers.size()));
	for (auto& layer : m_layers)
	{
		auto& frames = layer->GetAllKeyFrames();
		GD_ASSERT(frames.size() < std::numeric_limits<uint16_t>::max(), "overflow");
		es.Write(static_cast<uint16_t>(frames.size()));
		for (auto& frame : frames)
		{
			GD_ASSERT(frame->GetFrameIdx() < std::numeric_limits<uint16_t>::max(), "overflow");
			es.Write(static_cast<uint16_t>(frame->GetFrameIdx()));

			es.Write(static_cast<uint8_t>(frame->GetTween()));

			auto& nodes = frame->GetAllNodes();
			GD_ASSERT(nodes.size() < std::numeric_limits<uint16_t>::max(), "overflow");
			es.Write(static_cast<uint16_t>(nodes.size()));
			for (auto& node : nodes) {
				NodeSerializer::StoreToBin(node, dir, es);
			}
		}
	}
}

void N2CompAnim::LoadFromBin(const std::string& dir, bs::ImportStream& is)
{
	size_t layer_n = is.UInt16();
	for (size_t layer_i = 0; layer_i < layer_n; ++layer_i)
	{
		auto dst_layer = std::make_unique<anim::Layer>();
		size_t frame_n = is.UInt16();
		for (size_t frame_i = 0; frame_i < frame_n; ++frame_i)
		{
			size_t frame_idx = is.UInt16();
			bool tween = is.UInt8();

			auto dst_frame = std::make_unique<anim::KeyFrame>(frame_idx);
			dst_frame->SetTween(tween);

			size_t node_n = is.UInt16();
			for (size_t node_i = 0; node_i < node_n; ++node_i)
			{
				auto node = std::make_shared<n0::SceneNode>();
				NodeSerializer::LoadFromBin(node, dir, is);
				AddNode(node, dst_frame);
			}

			dst_layer->AddKeyFrame(dst_frame);
		}
	}
}

void N2CompAnim::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value layers_val;
	layers_val.SetArray();
	for (auto& layer : m_layers)
	{
		rapidjson::Value layer_val;
		layer_val.SetObject();

		rapidjson::Value frames_val;
		frames_val.SetArray();
		for (auto& frame : layer->GetAllKeyFrames())
		{
			rapidjson::Value frame_val;
			frame_val.SetObject();

			rapidjson::Value nodes_val;
			nodes_val.SetArray();
			for (auto& node : frame->GetAllNodes())
			{
				rapidjson::Value node_val;
				NodeSerializer::StoreToJson(node, dir, node_val, alloc);
				nodes_val.PushBack(node_val, alloc);
			}
			frame_val.AddMember("nodes", nodes_val, alloc);

			frame_val.AddMember("frame_idx", frame->GetFrameIdx(), alloc);
			frame_val.AddMember("tween", frame->GetTween(), alloc);

			frames_val.PushBack(frame_val, alloc);
		}

		layer_val.AddMember("frames", frames_val, alloc);
		layers_val.PushBack(layer_val, alloc);
	}

	val.AddMember("layers", layers_val, alloc);
}

void N2CompAnim::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto& layers_val = val["layers"];
	for (auto& src_layer : layers_val.GetArray())
	{
		auto dst_layer = std::make_unique<anim::Layer>();
		auto& frames_val = src_layer["frames"];
		for (auto& src_frame : frames_val.GetArray())
		{
			auto dst_frame = std::make_unique<anim::KeyFrame>(src_frame["frame_idx"].GetInt());
			for (auto& src_node : src_frame["nodes"].GetArray())
			{
				auto node = std::make_shared<n0::SceneNode>();
				NodeSerializer::LoadFromJson(node, dir, src_node);
				AddNode(node, dst_frame);
			}
			dst_frame->SetTween(src_frame["tween"].GetBool());
			dst_layer->AddKeyFrame(dst_frame);
		}
		m_layers.push_back(std::move(dst_layer));
	}
}

void N2CompAnim::StoreToMem(n2::CompAnim& comp) const
{
	for (auto& layer : m_layers) {
		comp.AddLayer(std::make_unique<anim::Layer>(*layer));
	}
}

void N2CompAnim::LoadFromMem(const n2::CompAnim& comp)
{
	m_layers.clear();
	for (auto& layer : comp.GetAllLayers()) {
		m_layers.push_back(std::make_unique<anim::Layer>(*layer));
	}
}

void N2CompAnim::AddNode(const n0::SceneNodePtr& node, anim::KeyFramePtr& frame)
{
	auto aabb = n2::AABBSystem::Instance()->GetBounding(*node);
	node->AddUniqueComp<n2::CompBoundingBox>(aabb);

	if (node->HasUniqueComp<n2::CompSharedPatch>())
	{
		auto& cpatch = node->GetUniqueComp<n2::CompSharedPatch>();
		cpatch.PatchToNode(node);
	}

	frame->AddNode(node);
}

}