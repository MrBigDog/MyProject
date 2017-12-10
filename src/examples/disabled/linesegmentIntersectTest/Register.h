#ifndef Register_h__
#define Register_h__

#include <osg/Referenced>
#include <osgDB/Options>

typedef std::map<std::string, osg::ref_ptr<osg::Image> > ImageMap;

class Register :public osg::Referenced
{
public:
	osgDB::Options* getDBOption() { return _dbOptions; }

	ImageMap& imageCache() { return _imageCache; }
	const ImageMap& imageCache() const { return _imageCache; }

private:
	Register();

private:
	osg::ref_ptr<osgDB::Options> _dbOptions;

	ImageMap _imageCache;
};
#endif // Register_h__
