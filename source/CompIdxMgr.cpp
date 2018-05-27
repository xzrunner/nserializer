#include "ns/CompIdxMgr.h"

#include <node0/CompComplex.h>
#include <node2/CompAnim.h>
#include <node2/CompImage.h>
#include <node2/CompMask.h>
#include <node2/CompScale9.h>
#include <node2/CompText.h>
#include <node3/CompModel.h>

#include <node0/CompFlags.h>
#include <node0/CompIdentity.h>
#include <node2/CompColorCommon.h>
#include <node2/CompColorMap.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompSharedPatch.h>
#include <node2/CompUniquePatch.h>
#include <node2/CompScissor.h>
#include <node2/CompScript.h>
#include <node3/CompAABB.h>
#include <node3/CompTransform.h>
#include <node3/CompModelInst.h>

namespace ns
{

CU_SINGLETON_DEFINITION(CompIdxMgr);

CompIdxMgr::CompIdxMgr()
{
}

CompIdx CompIdxMgr::CompTypeName2Idx(const std::string& name)
{
	if (name == n0::CompComplex::TYPE_NAME) {
		return COMP_N0_COMPLEX;
	} else if (name == n2::CompAnim::TYPE_NAME) {
		return COMP_N2_ANIM;
	} else if (name == n2::CompImage::TYPE_NAME) {
		return COMP_N2_IMAGE;
	} else if (name == n2::CompMask::TYPE_NAME) {
		return COMP_N2_MASK;
	} else if (name == n2::CompScale9::TYPE_NAME) {
		return COMP_N2_SCALE9;
	} else if (name == n2::CompText::TYPE_NAME) {
		return COMP_N2_TEXT;
	} else if (name == n3::CompModel::TYPE_NAME) {
		return COMP_N3_MODEL;
	} else if (name == n3::CompModelInst::TYPE_NAME) {
		return COMP_N3_MODEL_INST;
	} else if (name == n0::CompFlags::TYPE_NAME) {
		return COMP_N0_FLAGS;
	} else if (name == n0::CompIdentity::TYPE_NAME) {
		return COMP_N0_ID;
	} else if (name == n2::CompColorCommon::TYPE_NAME) {
		return COMP_N2_COL_COMMON;
	} else if (name == n2::CompColorMap::TYPE_NAME) {
		return COMP_N2_COL_MAP;
	} else if (name == n2::CompBoundingBox::TYPE_NAME) {
		return COMP_N2_BOUNDING_BOX;
	} else if (name == n2::CompTransform::TYPE_NAME) {
		return COMP_N2_TRANSFORM;
	} else if (name == n3::CompAABB::TYPE_NAME) {
		return COMP_N3_AABB;
	} else if (name == n3::CompTransform::TYPE_NAME) {
		return COMP_N3_TRANSFORM;
	} else if (name == n2::CompSharedPatch::TYPE_NAME) {
		return COMP_N2_SHARED_PATCH;
	} else if (name == n2::CompUniquePatch::TYPE_NAME) {
		return COMP_N2_UNIQUE_PATCH;
	} else if (name == n2::CompScissor::TYPE_NAME) {
		return COMP_N2_SCISSOR;
	} else if (name == n2::CompScript::TYPE_NAME) {
		return COMP_N2_SCRIPT;
	} else {
		auto itr = m_ext_name2idx.find(name);
		if (itr != m_ext_name2idx.end()) {
			return static_cast<CompIdx>(itr->second);
		} else {
			GD_REPORT_ASSERT("unknown type.");
			return COMP_INVALID;
		}
	}
}

}