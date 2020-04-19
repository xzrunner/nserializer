#include "ns/N2CompScript.h"
#include "ns/CompType.h"

#include <node2/CompScript.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompScript::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompScript::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompScript::LoadFromBin(const ur2::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompScript::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
}

void N2CompScript::LoadFromJson(const ur2::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();
}

void N2CompScript::StoreToMem(const ur2::Device& dev, n2::CompScript& comp) const
{
	comp.SetFilepath(m_filepath);
}

void N2CompScript::LoadFromMem(const n2::CompScript& comp)
{
	m_filepath = comp.GetFilepath();
}

}