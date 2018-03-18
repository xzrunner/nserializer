#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

#include <vector>

namespace n2 { class CompComplex; }

namespace ns
{

class N2CompComplex : public NodeComponent
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
	void StoreToMem(const n0::SceneNodePtr& node, n2::CompComplex& comp) const;
	void LoadFromMem(const n2::CompComplex& comp);

private:
	std::vector<n0::SceneNodePtr> m_nodes;

}; // N2CompComplex

}