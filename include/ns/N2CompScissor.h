#pragma once

#include "ns/NodeComponent.h"

#include <SM_Rect.h>

namespace n0 { class SceneNode; }
namespace n2 { class CompScissor; }

namespace ns
{

class N2CompScissor : public NodeComponent
{
public:
	//
	// bin
	//
	virtual size_t GetBinSize(const std::string& dir) const override;
	virtual void   StoreToBin(const std::string& dir, bs::ExportStream& es) const override;
	virtual void   LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is) override;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override;

	//
	// memory
	//
	void StoreToMem(const n0::SceneNode& node, n2::CompScissor& comp) const;
	void LoadFromMem(const n2::CompScissor& comp);

private:
	sm::rect m_rect;

}; // N2CompScissor

}