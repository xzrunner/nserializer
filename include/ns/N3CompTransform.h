#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>
#include <painting3/AABB.h>

namespace n3 { class CompTransform; }

namespace ns
{

class N3CompTransform : public NodeComponent
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
	void StoreToMem(const n0::SceneNodePtr& node, n3::CompTransform& comp) const;
	void LoadFromMem(const n3::CompTransform& comp);

private:
	sm::vec3       m_position;
	sm::Quaternion m_angle;
	sm::vec3       m_scale;

}; // N3CompTransform

}