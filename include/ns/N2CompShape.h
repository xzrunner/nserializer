#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

namespace ur { class Device; }
namespace n2 { class CompShape; }

namespace ns
{

class N2CompShape : public NodeComponent
{
public:
	//
	// bin
	//
	virtual size_t GetBinSize(const std::string& dir) const override;
	virtual void   StoreToBin(const std::string& dir, bs::ExportStream& es) const override;
	virtual void   LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is) override;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override;

	//
	// memory
	//
	void StoreToMem(const ur::Device& dev, n2::CompShape& comp) const;
	void LoadFromMem(const n2::CompShape& comp);

private:
	std::string m_type;

}; // N2CompShape

}