#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

namespace n2 { class CompTransform; }

namespace ns
{

class N2CompTransform : public NodeComponent
{
public:
	N2CompTransform();

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
	void StoreToMem(n2::CompTransform& comp) const;
	void LoadFromMem(const n2::CompTransform& comp);

private:
	float m_pos_x, m_pos_y;
	float m_angle;
	float m_scale_x, m_scale_y;
	float m_shear_x, m_shear_y;

}; // N2CompTransform

}