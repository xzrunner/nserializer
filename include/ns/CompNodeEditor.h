#pragma once

#include "ns/NodeComponent.h"

namespace ee0 { class CompNodeEditor; }

namespace ns
{

class CompNodeEditor : public NodeComponent
{
public:
	CompNodeEditor();

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
	void StoreToMem(ee0::CompNodeEditor& comp) const;
	void LoadFromMem(const ee0::CompNodeEditor& comp);

private:
	std::string m_filepath;
	std::string m_name;

	bool m_visible, m_editable;

}; // CompNodeEditor

}