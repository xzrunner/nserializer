#pragma once

#include <rapidjson/document.h>

#include <boost/noncopyable.hpp>

#include <string>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; class ExportStream; }

namespace ns
{

class NodeComponent : boost::noncopyable
{
public:
	virtual ~NodeComponent() {}

	//
	// bin
	//
	virtual size_t GetBinSize(const std::string& dir) const = 0;
	virtual void   StoreToBin(const std::string& dir, bs::ExportStream& es) const = 0;
	virtual void   LoadFromBin(const std::string& dir, bs::ImportStream& is) = 0;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const = 0;
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) = 0;

}; // NodeComponent

}