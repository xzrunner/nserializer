#pragma once

namespace ns
{

class RegistCallback
{
public:
	static void Init();

	template <typename TComp, typename TCompNS>
	static void AddUniqueCB();

	template <typename TComp, typename TCompNS>
	static void AddSharedCB();

	template <typename TComp, typename TCompNS>
	static void AddUniqueNullCB();

	template <typename TComp, typename TCompNS>
	static void AddAssetCB();

}; // RegistCallback

}

#include "ns/RegistCallback.inl"