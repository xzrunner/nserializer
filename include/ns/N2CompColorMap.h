#pragma once

#include "ns/NodeComponent.h"

namespace n0 { class SceneNode; }
namespace n2 { class CompColorMap; }

namespace ns
{

class N2CompColorMap : public NodeComponent
{
public:
	N2CompColorMap();

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
	void StoreToMem(const n0::SceneNode& node, n2::CompColorMap& comp) const;
	void LoadFromMem(const n2::CompColorMap& comp);

private:
	uint32_t m_rmap;
	uint32_t m_gmap;
	uint32_t m_bmap;

}; // N2CompColorMap

}