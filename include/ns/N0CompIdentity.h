#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

namespace ur2 { class Device; }
namespace n0 { class CompIdentity; }

namespace ns
{

class N0CompIdentity : public NodeComponent
{
public:
	N0CompIdentity();

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
	void StoreToMem(const ur2::Device& dev, n0::CompIdentity& comp) const;
	void LoadFromMem(const n0::CompIdentity& comp);

private:
	std::string m_filepath;
	std::string m_name;
	uint32_t    m_id;

}; // N0CompIdentity

}