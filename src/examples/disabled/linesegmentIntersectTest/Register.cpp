#include "Register.h"

#include <osgDB/Registry>
#include <osgDB/ReaderWriter>

namespace
{
	//纹理共享;
	class ReadAndShareImageCallback : public osgDB::ReadFileCallback
	{
	public:
		ReadAndShareImageCallback() { }
		virtual osgDB::ReaderWriter::ReadResult readImage(const std::string& filename, const osgDB::Options* options)
		{
			osg::ref_ptr<osg::Image> image = _imageCache[filename];
			if (image) return image;

			osgDB::ReaderWriter::ReadResult rr;
			rr = osgDB::Registry::instance()->readImageImplementation(filename, options);
			if (rr.success())
			{
				image = rr.getImage();
				_imageCache[filename] = image;
			}
			return image;
		}
	private:
		std::map<std::string, osg::ref_ptr<osg::Image> > _imageCache;//由于纹理数量不会很多，所以就不用LRU了。
	};
}


Register::Register()
{
	_dbOptions = new osgDB::Options;
	_dbOptions->setReadFileCallback(new ReadAndShareImageCallback);
}

