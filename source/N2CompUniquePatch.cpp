#include "ns/N2CompUniquePatch.h"
#include "ns/EditOpSerializer.h"

#include <node2/CompUniquePatch.h>

namespace ns
{

size_t N2CompUniquePatch::GetBinSize(const std::string& dir) const
{
	// todo
	return 0;
}

void N2CompUniquePatch::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	// todo
}

void N2CompUniquePatch::LoadFromBin(mm::LinearAllocator& alloc, const std::string& dir, bs::ImportStream& is)
{
	// todo
}

void N2CompUniquePatch::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value unique_val;
	unique_val.SetArray();
	for (auto& list : m_operators)
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

	val.AddMember("operators", unique_val, alloc);
}

void N2CompUniquePatch::LoadFromJson(mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	auto& op_array = val["operators"].GetArray();
	m_operators.reserve(op_array.Size());
	for (auto& op_val : op_array)
	{
		int node_id = op_val["node_id"].GetUint();
		n2::EditOpList edit_op_list(node_id);

		auto& ops_array = op_val["edit_op"].GetArray();
		for (auto& op_val : ops_array) {
			edit_op_list.AddEditOp(EditOpSerializer::FromJson(op_val));
		}

		m_operators.push_back(edit_op_list);
	}
}

void N2CompUniquePatch::StoreToMem(const n0::SceneNode& node, n2::CompUniquePatch& comp) const
{
	comp.SetAllOperators(m_operators);
}

void N2CompUniquePatch::LoadFromMem(const n2::CompUniquePatch& comp)
{
	m_operators = comp.GetAllOperators();
}

}