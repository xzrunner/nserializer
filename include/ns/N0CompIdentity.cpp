#include "ns/N0CompIdentity.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <node0/CompIdentity.h>

#include <boost/filesystem.hpp>

namespace ns
{

N0CompIdentity::N0CompIdentity()
	: m_id(0)
{
}

size_t N0CompIdentity::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	auto filepath_bin = boost::filesystem::path(m_filepath).replace_extension(".bin");
	std::string relative = boost::filesystem::relative(filepath_bin, dir).string();
	sz += bs::pack_size(relative);

	sz += bs::pack_size(m_name);

	sz += sizeof(uint32_t);		// id

	return sz;
}

void N0CompIdentity::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	auto filepath_bin = boost::filesystem::path(m_filepath).replace_extension(".bin");
	std::string relative = boost::filesystem::relative(filepath_bin, dir).string();
	es.Write(relative);

	es.Write(m_name);

	es.Write(static_cast<uint32_t>(m_id));
}

void N0CompIdentity::LoadFromBin(const std::string& dir, bs::ImportStream& is)
{
	auto relative = is.String();
	m_filepath = boost::filesystem::absolute(relative, dir).string();

	m_name = is.String();

	m_id = is.UInt32();
}

void N0CompIdentity::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);

	val.AddMember("name", rapidjson::Value(m_name.c_str(), alloc), alloc);
	val.AddMember("id", m_id, alloc);
}

void N0CompIdentity::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();

	m_name = val["name"].GetString();
	m_id   = val["id"].GetUint();
}

void N0CompIdentity::StoreToMem(n0::CompIdentity& comp) const
{
	comp.SetFilepath(m_filepath);
	comp.SetName(m_name);
	comp.SetID(m_id);
}

void N0CompIdentity::LoadFromMem(const n0::CompIdentity& comp)
{
	m_filepath = comp.GetFilepath();
	m_name     = comp.GetName();
	m_id       = comp.GetID();
}

}