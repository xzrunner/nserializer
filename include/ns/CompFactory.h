#pragma once

#include "ns/CompIdx.h"

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <functional>
#include <array>

namespace n0 { class NodeComp; }

namespace ns
{

class CompFactory
{
public:
	using CreateFunc = std::function<n0::NodeComp&(n0::SceneNodePtr&)>;
	using CreateAssetFunc = std::function<n0::CompAssetPtr()>;

	void AddCreateFunc(const std::string& name, const CreateFunc& func);
	void AddCreateAssetFunc(const std::string& name, const CreateAssetFunc& func);

	n0::NodeComp& Create(n0::SceneNodePtr& node, CompIdx comp_idx);
	n0::CompAssetPtr CreateAsset(CompIdx comp_idx);

	n0::CompAssetPtr CreateAsset(const std::string& filepath);

private:
	static const int MAX_COMP_NUM = 64;

	std::array<CreateFunc, MAX_COMP_NUM>      m_create;
	std::array<CreateAssetFunc, MAX_COMP_NUM> m_create_asset;

	CU_SINGLETON_DECLARATION(CompFactory);

}; // CompFactory

}