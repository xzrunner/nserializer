#pragma once

#include <node2/EditOp.h>

#include <rapidjson/document.h>

#include <memory>

namespace ns
{

class EditOpSerializer
{
public:
	static void ToJson(const n2::EditOp& op, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	static std::unique_ptr<n2::EditOp> FromJson(const rapidjson::Value& val);

private:
	static const char* GetTypeName(const n2::EditOp& op);

private:
#define X(Type) static void ToJson##Type(const n2::Type& op, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc);
	MAP_NODE_OPS_BASED_ON_TYPE(X)
#undef X

#define X(Type) static std::unique_ptr<n2::EditOp> FromJson##Type(const rapidjson::Value& val);
	MAP_NODE_OPS_BASED_ON_TYPE(X)
#undef X

}; // EditOpSerializer

}