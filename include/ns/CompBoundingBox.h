#pragma once

#include "ns/NodeComponent.h"

namespace n2 { class CompBoundingBox; }

namespace ns
{

class CompBoundingBox : public NodeComponent
{
public:
	CompBoundingBox();

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
	void StoreToMem(n2::CompBoundingBox& comp) const;
	void LoadFromMem(const n2::CompBoundingBox& comp);

private:
	float m_xmin, m_ymin, m_xmax, m_ymax;

}; // CompBoundingBox

}