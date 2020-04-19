#pragma once

#include "ns/NodeComponent.h"

#include <SM_Quaternion.h>
#include <node0/typedef.h>
#include <painting3/AABB.h>

namespace ur2 { class Device; }
namespace n3 { class CompTransform; }

namespace ns
{

class N3CompTransform : public NodeComponent
{
public:
	N3CompTransform();

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
	void StoreToMem(const ur2::Device& dev, n3::CompTransform& comp) const;
	void LoadFromMem(const n3::CompTransform& comp);

private:
	sm::vec3       m_position;
	sm::Quaternion m_angle;
	sm::vec3       m_scale;

}; // N3CompTransform

}