#include "ns/N2CompSprite2.h"
#include "ns/CompType.h"

#include <node2/CompSprite2.h>
#include <gum/SymbolPool.h>

#include <boost/filesystem.hpp>

namespace ns
{

size_t N2CompSprite2::GetBinSize(const std::string& dir) const
{
	// tood
	return 0;
}

void N2CompSprite2::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompSprite2::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompSprite2::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	std::string relative = boost::filesystem::relative(m_filepath, dir).string();
	val.AddMember("filepath", rapidjson::Value(relative.c_str(), alloc), alloc);
}

void N2CompSprite2::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto filepath = val["filepath"].GetString();
	m_filepath = boost::filesystem::absolute(filepath, dir).string();
}

void N2CompSprite2::StoreToMem(const n0::SceneNodePtr& node, n2::CompSprite2& comp) const
{
	comp.SetFilepath(m_filepath);

	auto sym = gum::SymbolPool::Instance()->Fetch(m_filepath.c_str());
	comp.SetSymbol(sym);
}

void N2CompSprite2::LoadFromMem(const n2::CompSprite2& comp)
{
	m_filepath = comp.GetFilepath();
}

}