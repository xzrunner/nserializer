#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>

#include <vector>

namespace ur2 { class Device; }
namespace n3 { class CompShape; }
namespace gs { class Shape3D; }

namespace ns
{

class N3CompShape : public NodeComponent
{
public:
    N3CompShape();

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
	void StoreToMem(const ur2::Device& dev, n3::CompShape& comp) const;
	void LoadFromMem(const n3::CompShape& comp);

private:
    std::vector<std::shared_ptr<gs::Shape3D>> m_shapes;

}; // N3CompShape

}