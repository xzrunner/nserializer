#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <rapidjson/document.h>

#include <functional>
#include <map>

namespace n0 { class NodeComponent; }

namespace ns
{

class CompSerializer
{
public:
	using ToJsonFunc = std::function<bool(const n0::NodeComponent&, const std::string&,
		rapidjson::Value&, rapidjson::MemoryPoolAllocator<>&)>;
	using FromJsonFunc = std::function<void(n0::SceneNodePtr&, const std::string&, const rapidjson::Value&)>;

	void AddToJsonFunc(const std::string& name, const ToJsonFunc& func);
	void AddFromJsonFunc(const std::string& name, const FromJsonFunc& func);

	bool ToJson(const n0::NodeComponent& comp, const std::string& dir,
		rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const;
	void FromJson(n0::SceneNodePtr& node, const std::string& name,
		const std::string& dir, const rapidjson::Value& val) const;

private:
	std::map<std::string, ToJsonFunc> m_to_json;

	std::map<std::string, FromJsonFunc> m_creator;

	CU_SINGLETON_DECLARATION(CompSerializer);

}; // CompSerializer

}