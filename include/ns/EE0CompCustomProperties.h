#pragma once

#include <ee0/CompCustomProperties.h>

#include "ns/NodeComponent.h"

#include <vector>

namespace ns
{

class EE0CompCustomProperties : public NodeComponent
{
public:
	EE0CompCustomProperties() {}

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
	void StoreToMem(ee0::CompCustomProperties& comp) const;
	void LoadFromMem(const ee0::CompCustomProperties& comp);

private:
	std::vector<ee0::CompCustomProperties::Property> m_props;

}; // EE0CompCustomProperties

}