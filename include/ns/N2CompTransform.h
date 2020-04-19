#pragma once

#include "ns/NodeComponent.h"

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace ur2 { class Device; }
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
	virtual void   LoadFromBin(const ur2::Device& dev, const std::string& dir, bs::ImportStream& is) override;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const ur2::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override;

	//
	// memory
	//
	void StoreToMem(const ur2::Device& dev, n2::CompTransform& comp) const;
	void LoadFromMem(const n2::CompTransform& comp);

private:
	sm::vec2 m_pos;
	float    m_angle;
	sm::vec2 m_scale;
	sm::vec2 m_shear;

}; // N2CompTransform

}