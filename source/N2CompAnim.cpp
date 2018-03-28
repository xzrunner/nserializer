#include "ns/N2CompAnim.h"
#include "ns/NodeSerializer.h"

#include <node0/SceneNode.h>
#include <node2/CompAnim.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <node2/CompSharedPatch.h>
#include <anim/Layer.h>
#include <anim/KeyFrame.h>

namespace ns
{

size_t N2CompAnim::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompAnim::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompAnim::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
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
				NodeSerializer::StoreNodeToJson(node, dir, node_val, alloc);
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

				NodeSerializer::LoadNodeFromJson(node, dir, src_node);

				auto& casset = node->GetSharedComp<n0::CompAsset>();
				node->AddUniqueComp<n2::CompBoundingBox>(casset.GetBounding());

				if (node->HasUniqueComp<n2::CompSharedPatch>())
				{
					auto& cpatch = node->GetUniqueComp<n2::CompSharedPatch>();
					cpatch.PatchToNode(node);
				}

				dst_frame->AddNode(node);
			}
			dst_frame->SetTween(src_frame["tween"].GetBool());
			dst_layer->AddKeyFrame(dst_frame);
		}
		m_layers.push_back(std::move(dst_layer));
	}
}

void N2CompAnim::StoreToMem(const n0::SceneNodePtr& node, n2::CompAnim& comp) const
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

}