#include "ns/RegistCallback.h"
#include "ns/CompSerializer.h"
#include "ns/CompNoSerialize.h"

#include "ns/N0CompComplex.h"
#include <node0/CompComplex.h>

#include "ns/N2CompAnim.h"
#include <node2/CompAnim.h>
#include "ns/N2CompColorCommon.h"
#include <node2/CompColorCommon.h>
#include "ns/N2CompColorMap.h"
#include <node2/CompColorMap.h>

#include "ns/N2CompTransform.h"
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include "ns/N2CompUniquePatch.h"
#include <node2/CompUniquePatch.h>
#include "ns/N2CompSharedPatch.h"
#include <node2/CompSharedPatch.h>
#include "ns/N2CompScissor.h"
#include <node2/CompScissor.h>
#include "ns/N2CompScript.h"
#include <node2/CompScript.h>
#include "ns/N2CompScript.h"

// todo
#include <node2/EditOp.h>

#include "ns/N0CompIdentity.h"
#include <node0/CompIdentity.h>
#include "ns/N0CompComplex.h"
#include <node0/CompComplex.h>

#include "ns/N2CompImage.h"
#include <node2/CompImage.h>
#include "ns/N2CompMask.h"
#include <node2/CompMask.h>
#include "ns/N2CompText.h"
#include <node2/CompText.h>
#include "ns/N2CompScale9.h"
#include <node2/CompScale9.h>

#include "ns/N3CompAABB.h"
#include <node3/CompAABB.h>
#include "ns/N3CompTransform.h"
#include <node3/CompTransform.h>
#include "ns/N3CompModel.h"
#include <node3/CompModel.h>
#include "ns/N3CompModelInst.h"
#include <node3/CompModelInst.h>

#include "ns/N0CompFlags.h"
#include <node0/CompFlags.h>

#include <node0/SceneNode.h>
#include <memmgr/LinearAllocator.h>
#include <anim/Layer.h>
#include <anim/KeyFrame.h>

namespace ns
{

void RegistCallback::Init()
{
	AddUniqueCB<n0::CompIdentity, N0CompIdentity>();

	AddUniqueCB<n2::CompColorCommon, N2CompColorCommon>();
	AddUniqueCB<n2::CompColorMap, N2CompColorMap>();

	AddUniqueCB<n2::CompTransform, N2CompTransform>();
	AddUniqueNullCB<n2::CompBoundingBox, CompNoSerialize>();
	AddUniqueCB<n2::CompUniquePatch, N2CompUniquePatch>();
	AddUniqueCB<n2::CompSharedPatch, N2CompSharedPatch>();
	AddUniqueCB<n2::CompScissor, N2CompScissor>();
	AddUniqueCB<n2::CompScript, N2CompScript>();

	AddSharedCB<n0::CompComplex, N0CompComplex>();
	AddSharedCB<n2::CompAnim, N2CompAnim>();
	AddSharedCB<n2::CompImage, N2CompImage>();
	AddSharedCB<n2::CompMask, N2CompMask>();
	AddSharedCB<n2::CompText, N2CompText>();
	AddSharedCB<n2::CompScale9, N2CompScale9>();

	AddUniqueCB<n3::CompAABB, N3CompAABB>();
	AddUniqueCB<n3::CompTransform, N3CompTransform>();
	AddSharedCB<n3::CompModel, N3CompModel>();
	AddUniqueCB<n3::CompModelInst, N3CompModelInst>();

	AddUniqueCB<n0::CompFlags, N0CompFlags>();

	AddAssetCB<n0::CompComplex>();
	AddAssetCB<n2::CompAnim>();
	AddAssetCB<n2::CompImage>();
	AddAssetCB<n2::CompMask>();
	AddAssetCB<n2::CompText>();
	AddAssetCB<n2::CompScale9>();
}

}