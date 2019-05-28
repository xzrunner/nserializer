#include "ns/CompFactory.h"
#include "ns/CompIdxMgr.h"
#include "ns/CompSerializer.h"

#include <bs/ImportStream.h>
#include <js/RapidJsonHelper.h>
#include <sx/ResFileHelper.h>
#include <node0/CompComplex.h>
#include <node2/CompImage.h>
#include <node3/CompModel.h>
#include <node3/CompImage3D.h>
#include <model/Model.h>
#include <facade/ResPool.h>
#include <facade/Image.h>
#include <facade/Image3D.h>

#include <boost/filesystem.hpp>

namespace ns
{

CU_SINGLETON_DEFINITION(CompFactory);

CompFactory::CompFactory()
{
}

void CompFactory::AddCreateFunc(const std::string& name, const CompFactory::CreateFunc& func)
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(name);
	m_create[idx] = func;
}

void CompFactory::AddCreateAssetFunc(const std::string& name, const CreateAssetFunc& func)
{
	size_t idx = CompIdxMgr::Instance()->CompTypeName2Idx(name);
	m_create_asset[idx] = func;
}

n0::NodeComp& CompFactory::Create(n0::SceneNodePtr& node, size_t comp_idx)
{
	return m_create[comp_idx](node);
}

n0::CompAssetPtr CompFactory::CreateAsset(size_t comp_idx)
{
	return m_create_asset[comp_idx]();
}

n0::CompAssetPtr CompFactory::CreateAsset(const std::string& filepath, bool force_reload)
{
    n0::CompAssetPtr casset = nullptr;
    if (!force_reload) {
        casset = facade::ResPool::Instance().Query<n0::CompAsset>(filepath);
        if (casset) {
            return casset;
        }
    }

	auto type = sx::ResFileHelper::Type(filepath);
	switch (type)
	{
	case sx::RES_FILE_IMAGE:
	{
		auto img = facade::ResPool::Instance().Fetch<facade::Image>(filepath);
		auto cimage = std::make_shared<n2::CompImage>();
		cimage->SetFilepath(filepath);
		cimage->SetTexture(img->GetTexture());
		casset = cimage;
	}
		break;
	case sx::RES_FILE_IMAGE3D:
	{
		auto img = facade::ResPool::Instance().Fetch<facade::Image3D>(filepath);
		auto cimage = std::make_shared<n3::CompImage3D>();
		cimage->SetFilepath(filepath);
		cimage->SetTexture(img->GetTexture());
		casset = cimage;
	}
		break;
	case sx::RES_FILE_JSON:
	{
		rapidjson::Document doc;
		js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

		auto type = doc[CompSerializer::Instance()->COMP_TYPE_NAME].GetString();
		casset = CreateAsset(CompIdxMgr::Instance()->CompTypeName2Idx(type));

		auto dir = boost::filesystem::path(filepath).parent_path().string();
		ns::CompSerializer::Instance()->FromJson(*casset, dir, doc);
	}
		break;
	case sx::RES_FILE_BIN:
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::ate);
		size_t size = static_cast<size_t>(file.tellg());
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (file.read(buffer.data(), size))
		{
			casset = CreateAsset(buffer[0]);

			auto dir = boost::filesystem::path(filepath).parent_path().string();
			ns::CompSerializer::Instance()->FromBin(*casset, dir, bs::ImportStream(&buffer[0], size));
		}
	}
		break;
	case sx::RES_FILE_MODEL:
	{
		auto model = facade::ResPool::Instance().Fetch<model::Model>(filepath);
		auto cmodel = std::make_shared<n3::CompModel>();
		cmodel->SetFilepath(filepath);
		cmodel->SetModel(model);
		casset = cmodel;
	}
		break;
    case sx::RES_FILE_SHADER:
        break;
    case sx::RES_FILE_ASSET:
        break;
	case sx::RES_FILE_MAP:
		casset = std::make_shared<n0::CompComplex>();
		break;
	}

	if (casset) {
		facade::ResPool::Instance().Insert<n0::CompAsset>(filepath, casset);
	}

	return casset;
}

n0::CompAssetPtr CompFactory::CreateAsset(const rapidjson::Value& val,
                                          const std::string& dir)
{
	auto type = val[CompSerializer::Instance()->COMP_TYPE_NAME].GetString();
    n0::CompAssetPtr casset = CreateAsset(CompIdxMgr::Instance()->CompTypeName2Idx(type));

	ns::CompSerializer::Instance()->FromJson(*casset, dir, val);

	return casset;
}

}