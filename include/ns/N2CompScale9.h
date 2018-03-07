#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

#include <vector>

namespace n2 { class CompScale9; }

namespace ns
{

class N2CompScale9 : public NodeComponent
{
public:
	N2CompScale9();

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
	void StoreToMem(const n0::SceneNode& node, n2::CompScale9& comp) const;
	void LoadFromMem(const n2::CompScale9& comp);

private:
	int m_type;

	std::vector<n0::SceneNodePtr> m_grids;

	float m_width, m_height;

}; // N2CompScale9

}