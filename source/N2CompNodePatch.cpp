#include "ns/N2CompNodePatch.h"
#include "ns/EditOpSerializer.h"

#include <node2/CompNodePatch.h>

namespace ns
{

size_t N2CompNodePatch::GetBinSize(const std::string& dir) const
{
	// todo
	return 0;
}

void N2CompNodePatch::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompNodePatch::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompNodePatch::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value unique_val;
	unique_val.SetArray();
	for (auto& list : m_unique_op)
	{
		rapidjson::Value list_val;
		list_val.SetObject();
		list_val.AddMember("node_id", list.GetNodeID(), alloc);
		
		rapidjson::Value ops_val;
		ops_val.SetArray();
		auto& all_edit_op = list.GetAllEditOp();
		for (auto& op : all_edit_op)
		{
			rapidjson::Value op_val;
			EditOpSerializer::ToJson(*op, op_val, alloc);
			ops_val.PushBack(op_val, alloc);
		}
		list_val.AddMember("edit_op", ops_val, alloc);

		unique_val.PushBack(list_val, alloc);
	}

	val.AddMember("unique_op", unique_val, alloc);
}

void N2CompNodePatch::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto& unique_op_array = val["unique_op"].GetArray();
	m_unique_op.reserve(unique_op_array.Size());
	for (auto& unique_op_val : unique_op_array)
	{
		int node_id = unique_op_val["node_id"].GetUint();
		n2::EditOpList edit_op_list(node_id);

		auto& ops_array = unique_op_val["edit_op"].GetArray();
		for (auto& op_val : ops_array) {
			edit_op_list.AddEditOp(EditOpSerializer::FromJson(op_val));
		}

		m_unique_op.push_back(edit_op_list);
	}
}

void N2CompNodePatch::StoreToMem(const n0::SceneNode& node, n2::CompNodePatch& comp) const
{
	comp.SetAllUniqueOP(m_unique_op);
}

void N2CompNodePatch::LoadFromMem(const n2::CompNodePatch& comp)
{
	m_unique_op = comp.GetAllUniqueOP();
}

}