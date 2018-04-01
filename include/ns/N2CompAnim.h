#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>
#include <anim/typedef.h>

#include <vector>

namespace n2 { class CompAnim; }

namespace ns
{

class N2CompAnim : public NodeComponent
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
	void StoreToMem(n2::CompAnim& comp) const;
	void LoadFromMem(const n2::CompAnim& comp);

private:
	std::vector<anim::LayerPtr> m_layers;

}; // N2CompAnim

}