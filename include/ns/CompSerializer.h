#pragma once

#include "ns/CompIdx.h"

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <rapidjson/document.h>

#include <functional>
#include <map>
#include <array>

namespace n0 { class NodeComp; }
namespace bs { class ExportStream; class ImportStream; }

namespace ns
{

class CompSerializer
{
public:
	using ToJsonFunc = std::function<bool(const n0::NodeComp&, const std::string&,
		rapidjson::Value&, rapidjson::MemoryPoolAllocator<>&, bool)>;
	using FromJsonFunc = std::function<void(n0::NodeComp&, const std::string&, const rapidjson::Value&)>;

	void AddToJsonFunc(const std::string& name, const ToJsonFunc& func, bool replace = false);
	void AddFromJsonFunc(const std::string& name, const FromJsonFunc& func, bool replace = false);

	bool ToJson(const n0::NodeComp& comp, const std::string& dir, rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc, bool skip_asset = true) const;
	void FromJson(n0::NodeComp& comp, const std::string& dir, const rapidjson::Value& val) const;

	using GetBinSizeFunc = std::function<size_t(const n0::NodeComp&, const std::string&)>;
	using ToBinFunc = std::function<void(const n0::NodeComp&, const std::string&, bs::ExportStream&)>;
	using FromBinFunc = std::function<void(n0::NodeComp&, const std::string&, bs::ImportStream& is)>;

	void AddGetBinSizeFunc(const std::string& name, const GetBinSizeFunc& func);
	void AddToBinFunc(const std::string& name, const ToBinFunc& func);
	void AddFromBinFunc(const std::string& name, const FromBinFunc& func);

	size_t GetBinSize(const n0::NodeComp& comp, const std::string& dir) const;
	void ToBin(const n0::NodeComp& comp, const std::string& dir, bs::ExportStream& es) const;
	void FromBin(n0::NodeComp& comp, const std::string& dir, bs::ImportStream& is, CompIdx idx = COMP_INVALID) const;

public:
	static const char* COMP_TYPE_NAME;

private:
	static const int MAX_COMP_NUM = 64;

	std::map<std::string, ToJsonFunc>        m_to_json;
	std::map<std::string, FromJsonFunc>      m_from_json;

	std::array<GetBinSizeFunc, MAX_COMP_NUM> m_get_bin_sz;
	std::array<ToBinFunc, MAX_COMP_NUM>      m_to_bin;
	std::array<FromBinFunc, MAX_COMP_NUM>    m_from_bin;

	CU_SINGLETON_DECLARATION(CompSerializer);

}; // CompSerializer

}