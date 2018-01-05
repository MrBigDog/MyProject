/************************************************************************/
// GeniusWorld模型解析插件   [12/1/2017 BigDog]
/************************************************************************/

#include <osg/io_utils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgEarth/URI>

struct ReaderWriterM : public osgDB::ReaderWriter
{
	ReaderWriterM()
	{
		this->supportsExtension("m", "gw model reader");
	}

	const char* className() const { return "gw model gen reader"; }

	bool acceptsExtension(const std::string& extension) const
	{
		return osgDB::equalCaseInsensitive(extension, "m");
	}

	ReadResult readObject(const std::string& filename, const osgDB::Options* options) const
	{
		return readNode(filename, options);
	}

	ReadResult readNode(const std::string& filename, const osgDB::Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(filename)))
			return ReadResult::FILE_NOT_HANDLED;

		osgEarth::URI url(filename);
		std::string ss = url.getString();
		std::stringstream nodebuffer;
		nodebuffer << ss;

		ReadResult rr =
			osgDB::Registry::instance()->getReaderWriterForExtension("zip")->readNode(nodebuffer, options);

		return rr;
		//return readNode(newname, options);
	}
};

REGISTER_OSGPLUGIN(m, ReaderWriterM)