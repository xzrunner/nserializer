#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

namespace n0 { class CompFlags; }

namespace ns
{

class N0CompFlags : public NodeComponent
{
public:
	N0CompFlags();

	//
	// bin
	//
	virtual size_t GetBinSize(const std::string& dir) const override;
	virtual void   StoreToBin(const std::string& dir, bs::ExportStream& es) const override;
	virtual void   LoadFromBin(const std::string& dir, bs::ImportStream& is) override;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override;

	//
	// memory
	//
	void StoreToMem(n0::CompFlags& comp) const;
	void LoadFromMem(const n0::CompFlags& comp);

private:
	bool m_visible, m_editable;

}; // N0CompFlags

}