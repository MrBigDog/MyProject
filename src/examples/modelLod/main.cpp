#include <osg/io_utils>
#include <osg/ShapeDrawable>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgGA/StateSetManipulator>
#include <osgUtil/Simplifier>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
//#include "FileUtils.h"
#include "ImageUtils.h"

class TextureAndImageVisitor : public osg::NodeVisitor
{
public:
	TextureAndImageVisitor(int level)
		: osg::NodeVisitor()
		, _level(level)
	{
		setNodeMaskOverride(~0L);
		setTraversalMode(TRAVERSE_ALL_CHILDREN);
		std::stringstream ss;
		ss << _level;
		_nameExt = ss.str();
	}

	~TextureAndImageVisitor() { }

public:
	void apply(osg::Texture& texture)
	{
		for (unsigned k = 0; k < texture.getNumImages(); ++k)
		{
			osg::Image* image = texture.getImage(k);
			if (!image) continue;

			//转gltf后纹理名字可能重复，所以在这里改一下名字;
			std::string imageName = image->getFileName();
			std::string sname = osgDB::getSimpleFileName(imageName);
			std::string fileExt = osgDB::getFileExtensionIncludingDot(sname);
			std::string snameWithoutExt = osgDB::getNameLessExtension(sname);
			std::string newFileName = snameWithoutExt + _nameExt + fileExt;
			image->setFileName(newFileName);

			//纹理抽稀;
			osg::ref_ptr<osg::Image> newImage = 0L;
			ImageUtils::resizeImage(image, image->s() / 2, image->t() / 2, newImage);

			texture.setImage(k, newImage);
			texture.dirtyTextureObject();
		}
	}

public:
	void apply(osg::Node& node)
	{
		if (node.getStateSet())
		{
			apply(*node.getStateSet());
		}
		traverse(node);
	}

	void apply(osg::Geode& geode)
	{
		if (geode.getStateSet())
		{
			apply(*geode.getStateSet());
		}
		for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
		{
			apply(*geode.getDrawable(i));
		}
	}

	void apply(osg::Drawable& drawable)
	{
		if (drawable.getStateSet())
		{
			apply(*drawable.getStateSet());
		}
	}

	void apply(osg::StateSet& stateSet)
	{
		osg::StateSet::TextureAttributeList& a = stateSet.getTextureAttributeList();
		for (osg::StateSet::TextureAttributeList::iterator i = a.begin(); i != a.end(); ++i)
		{
			osg::StateSet::AttributeList& b = *i;
			for (osg::StateSet::AttributeList::iterator j = b.begin(); j != b.end(); ++j)
			{
				osg::StateAttribute* sa = j->second.first.get();
				if (sa)
				{
					osg::Texture* tex = dynamic_cast<osg::Texture*>(sa);
					if (tex)
					{
						apply(*tex);
					}
				}
			}
		}
	}

private:
	int _level;
	std::string _nameExt;
};

//class DirectoryVisitor
//{
//public:
//	DirectoryVisitor() {}
//
//	virtual void handleFile(const std::string& filename)
//	{
//		filenames.push_back(filename);
//	}
//
//	virtual bool handleDir(const std::string& path) { return true; }
//
//	virtual void traverse(const std::string& path)
//	{
//		if (osgDB::fileType(path) == osgDB::DIRECTORY)
//		{
//			if (handleDir(path))
//			{
//				osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
//				for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
//				{
//					if (f->compare(".") == 0 || f->compare("..") == 0)
//						continue;
//
//					std::string filepath = osgDB::concatPaths(path, *f);
//					traverse(filepath);
//				}
//			}
//		}
//		else if (osgDB::fileType(path) == osgDB::REGULAR_FILE)
//		{
//			handleFile(path);
//		}
//	}
//
//
//	void writeToLocal(const std::string& resName)
//	{
//		std::ofstream out(resName/*"filename_list.txt"*/);
//		for (std::vector<std::string>::const_iterator it = filenames.begin(); it != filenames.end(); ++it)
//		{
//			out << *it << std::endl;
//		}
//	}
//
//private:
//	std::vector< std::string > filenames;
//};


int main()
{
	std::string originalFileName = "";//模型的名字

	int level = 3;
	osg::ref_ptr<osg::Node> testNode = osgDB::readNodeFile(originalFileName);

	osg::ComputeBoundsVisitor cbv;
	testNode->accept(cbv);

	const osg::BoundingBox& bb = cbv.getBoundingBox();
	const osg::Vec3d& cc = bb.center();
	float radius = bb.radius();

	std::stringstream ss;
	ss << "_" << 0;

	std::string nameWithoutExt = osgDB::getNameLessExtension(originalFileName);
	std::string newFileName = nameWithoutExt + ss.str() + ".osgb";

	std::string currentFileName = "";

	osg::ref_ptr<osg::Group> root = new osg::Group;
	for (unsigned int i = 0; i < level; ++i)
	{
		std::stringstream ss;
		ss << "_" << i + 1;

		std::string nextLevelName = nameWithoutExt + ss.str() + ".osgb";
		osg::ref_ptr<osg::PagedLOD> pl = new osg::PagedLOD;
		pl->addChild(testNode);
		pl->setFileName(1, nextLevelName);
		pl->setCenter(cc);
		pl->setRadius(radius);
		pl->setRange(0, radius*6.0f, FLT_MAX);
		pl->setRange(1, 0, radius*6.0f);

		TextureAndImageVisitor tv(i);
		testNode->accept(tv);

		osgDB::writeNodeFile(*pl, currentFileName/*, new osgDB::Options("WriteImageHint=WriteOut")*/);
		currentFileName = nextLevelName;
	}

	return 0;
}