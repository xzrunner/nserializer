#include "ns/ResFileHelper.h"

#include <boost/filesystem.hpp>

namespace ns
{

ResFileType ResFileHelper::Type(const std::string& filepath)
{
	ResFileType type = FILE_UNKNOWN;
	auto ext = boost::filesystem::extension(filepath);
	if (ext == ".png" || ext == ".jpg" || ext == ".bmp" || ext == ".ppm" || ext == ".pvr" || ext == ".pkm") {
		type = FILE_IMAGE;
	} else if (ext == ".json") {
		type = FIME_JSON;
	}
	return type;
}

}