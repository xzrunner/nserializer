#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

namespace ur2 { class Device; }
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
	virtual void   LoadFromBin(const ur2::Device& dev, const std::string& dir, bs::ImportStream& is) override;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const ur2::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override;

	//
	// memory
	//
	void StoreToMem(const ur2::Device& dev, n2::CompColorMap& comp) const;
	void LoadFromMem(const n2::CompColorMap& comp);

private:
	uint32_t m_rmap;
	uint32_t m_gmap;
	uint32_t m_bmap;

}; // N2CompColorMap

}