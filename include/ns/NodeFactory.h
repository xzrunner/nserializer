#pragma once

#include <node0/typedef.h>

#include <rapidjson/document.h>

namespace n0 { class CompIdentity; }
namespace ur2 { class Device; }

namespace ns
{

class NodeFactory
{
public:
	static n0::SceneNodePtr Create(const ur2::Device& dev, const std::string& filepath);
	static n0::SceneNodePtr Create2D();
    static n0::SceneNodePtr Create3D();

private:
	static n0::SceneNodePtr CreateFromCommon(const ur2::Device& dev, const std::string& filepath);
	static n0::SceneNodePtr CreateFromImage(const ur2::Device& dev, const std::string& filepath);
	static n0::SceneNodePtr CreateFromImage3D(const ur2::Device& dev, const std::string& filepath);
	static n0::SceneNodePtr CreateFromModel(const ur2::Device& dev, const std::string& filepath);
	static n0::SceneNodePtr CreateFromScript(const std::string& filepath);

	static void InitCompId(n0::CompIdentity& cid, const std::string& filepath);

}; // NodeFactory

}