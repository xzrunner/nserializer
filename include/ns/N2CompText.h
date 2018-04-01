#pragma once

#include "ns/NodeComponent.h"

#include <node0/typedef.h>
#include <painting2/Text.h>

namespace n2 { class CompText; }

namespace ns
{

class N2CompText : public NodeComponent
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
	void StoreToMem(n2::CompText& comp) const;
	void LoadFromMem(const n2::CompText& comp);

private:
	static void StoreTBToJson(const pt2::Textbox& tb, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	static void LoadTBFromJson(pt2::Textbox& tb, mm::LinearAllocator& alloc, const rapidjson::Value& val);

	static void StoreColorToJson(const pt2::GradientColor& col, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	static void LoadColorFromJson(pt2::GradientColor& col, mm::LinearAllocator& alloc, const rapidjson::Value& val);

private:
	pt2::Text m_text;

}; // N2CompText

}