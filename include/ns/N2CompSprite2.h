#pragma once

#include "ns/NodeComponent.h"

namespace n0 { class SceneNode; }
namespace n2 { class CompSprite2; }

namespace ns
{

class N2CompSprite2 : public NodeComponent
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
	void StoreToMem(const n0::SceneNode& node, n2::CompSprite2& comp) const;
	void LoadFromMem(const n2::CompSprite2& comp);

private:
	std::string m_filepath;

}; // N2CompSprite2

}