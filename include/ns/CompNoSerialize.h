#pragma once

#include "ns/NodeComponent.h"

namespace ns
{

class CompNoSerialize : public NodeComponent
{
public:
	//
	// bin
	//
	virtual size_t GetBinSize(const std::string& dir) const override { return 0; }
	virtual void   StoreToBin(const std::string& dir, bs::ExportStream& es) const override {}
	virtual void   LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is) override {}

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override {}
	virtual void LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override {}

}; // CompNoSerialize

}