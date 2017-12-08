/************************************************************************/
//  GeniusWorldÎÆÀí½âÎö²å¼þ;          [12/1/2017 BigDog]                                      
/************************************************************************/

#include <osg/io_utils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgEarth/URI>

struct ReaderWriterT : public osgDB::ReaderWriter
{
	ReaderWriterT()
	{
		this->supportsExtension("t", "gw texture reader");
	}

	const char* className() const { return "gw texture reader"; }

	bool acceptsExtension(const std::string& extension) const
	{
		return osgDB::equalCaseInsensitive(extension, "t");
	}

	ReadResult readObject(const std::string& filename, const osgDB::Options* options) const
	{
		return readImage(filename, options);
	}

	ReadResult readImage(const std::string& filename, const osgDB::Options* options) const
	{
		if (!acceptsExtension(osgDB::getFileExtension(filename)))
			return ReadResult::FILE_NOT_HANDLED;


		osgEarth::URI url(filename);
		std::string ss = url.getString();
		std::stringstream nodebuffer;
		nodebuffer << ss;

		ReadResult rr =
			osgDB::Registry::instance()->getReaderWriterForExtension("zip")->readImage(nodebuffer, options);
		//std::string newname = osgDB::getNameLessExtension(filename) + ".zip";

		return rr;
	}
};

REGISTER_OSGPLUGIN(t, ReaderWriterT)