#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include "FileUtils.h"
#include "StringUtils.h"

#include <windows.h>

class GeodeVisitor :public osg::NodeVisitor
{
public:
	GeodeVisitor(const std::string& name)
		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, _name(name)
	{}

	void apply(osg::Geode& geode)
	{
		geode.setName(_name);
	}

private:
	std::string _name;
};

class TextureAndImageVisitor : public osg::NodeVisitor
{
public:
	TextureAndImageVisitor(const std::string& rootPath)
		: osg::NodeVisitor()
		, _outpath(rootPath)
	{
		setNodeMaskOverride(~0L);
		setTraversalMode(TRAVERSE_ALL_CHILDREN);
	}
	virtual ~TextureAndImageVisitor() { }

public:
	virtual void apply(osg::Texture& texture)
	{
		for (unsigned k = 0; k < texture.getNumImages(); ++k)
		{
			osg::Image* image = texture.getImage(k);
			if (image)
			{
				apply(*image);
			}
		}
	}

	virtual void apply(osg::Image& image)
	{
		std::string filename = osgDB::concatPaths(_outpath, image.getFileName());
		osgDB::writeImageFile(image, filename);
	}

public:
	virtual void apply(osg::Node& node)
	{
		if (node.getStateSet())
			apply(*node.getStateSet());

		traverse(node);
	}

	virtual void apply(osg::Geode& geode)
	{
		if (geode.getStateSet())
			apply(*geode.getStateSet());

		for (unsigned i = 0; i < geode.getNumDrawables(); ++i) {
			apply(*geode.getDrawable(i));
			//if (geode.getDrawable(i) && geode.getDrawable(i)->getStateSet())
			//    apply(*geode.getDrawable(i)->getStateSet());
		}
		//traverse(geode);
	}

	virtual void apply(osg::Drawable& drawable)
	{
		if (drawable.getStateSet())
			apply(*drawable.getStateSet());
		//traverse(drawable);
	}

	virtual void apply(osg::StateSet& stateSet)
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
	std::string _outpath;
};

class DirectoryVisitor
{
public:
	DirectoryVisitor::DirectoryVisitor(const std::string& inPath, const std::string& outPath, const std::string& ext, int lodLevel, int maxDirNum)
		: _inPath(inPath)
		, _outPath(outPath)
		, _level(lodLevel)
		, _ext(ext)
		, _maxDirNum(maxDirNum)
		, _dirNum(0)
		, _groupNum(0)
		, _root(new osg::Group())
	{
		if (!gwUtil::startsWith(_ext, "."))
		{
			_ext = "." + _ext;
		}

		if (osgDB::fileType(inPath) == osgDB::DIRECTORY)
		{
			traverseDirectory(inPath);
			writeLastModel();
		}
		else
		{
			handleFile(inPath);
		}
	}

private:
	void DirectoryVisitor::traverseDirectory(const std::string& path)
	{
		if (osgDB::fileType(path) == osgDB::DIRECTORY)
		{
			if (!handleDir(path)) return;

			//osg::ref_ptr<osg::Group> root = new osg::Group;

			unsigned int filesInCurrentDir = 0;
			osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
			for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
			{
				if (f->compare(".") == 0 || f->compare("..") == 0)
					continue;

				std::string filepath = osgDB::concatPaths(path, *f);

				if (osgDB::fileType(filepath) == osgDB::REGULAR_FILE)
				{
					if (handleFile(filepath, _root))
					{
						filesInCurrentDir++;
					}
				}
				else
				{
					traverseDirectory(filepath);
				}
			}
			if (filesInCurrentDir > 1)
			{
				_dirNum++;
			}

			if (_root->getNumChildren() > 0 && _dirNum >= _maxDirNum)
			{
				std::stringstream ss;
				ss << _groupNum;

				std::string newPath = osgDB::concatPaths(_outPath, ss.str());
				std::string newName = osgDB::concatPaths(newPath, ss.str() + _ext);

				if (!osgDB::fileExists(newName))
				{
					gwUtil::makeDirectoryForFile(newName);
				}
				osgDB::writeNodeFile(*_root, newName);

				if (_ext == ".obj" || _ext == "obj")
				{
					TextureAndImageVisitor tv(newPath);
					_root->accept(tv);
				}

				_root->removeChildren(0, _root->getNumChildren());

				_groupNum++;
				_dirNum = 0;
			}
		}
	}

	void writeLastModel()
	{
		if (_root->getNumChildren() <= 0) return;

		std::stringstream ss;
		ss << _groupNum;

		std::string newPath = osgDB::concatPaths(_outPath, ss.str());
		std::string newName = osgDB::concatPaths(newPath, ss.str() + _ext);

		if (!osgDB::fileExists(newName))
		{
			gwUtil::makeDirectoryForFile(newName);
		}
		osgDB::writeNodeFile(*_root, newName);
		if (_ext == ".obj" || _ext == "obj")
		{
			TextureAndImageVisitor tv(newPath);
			_root->accept(tv);
		}

		_root->removeChildren(0, _root->getNumChildren());

		_groupNum++;
		_dirNum = 0;
	}

	void handleFile(const std::string& filename)
	{
		osg::ref_ptr<osg::Group> root = new osg::Group;

		handleFile(filename, root);

		if (root->getNumChildren() > 0)
		{
			std::string filenameWithoutExt = osgDB::getNameLessExtension(filename);

			gwUtil::StringVector pathElements;
			osgDB::getPathElements(filenameWithoutExt, pathElements);
			std::string rootPath = filenameWithoutExt.substr(_inPath.size(), filenameWithoutExt.size());
			std::string newName = rootPath + _ext;
			osgDB::writeNodeFile(*root, newName);
			if (_ext == ".obj" || _ext == "obj")
			{
				TextureAndImageVisitor tv(rootPath);
				root->accept(tv);
			}
		}
	}

	bool handleFile(const std::string& filename, osg::Group* root)
	{
		std::string fileexten = osgDB::convertToLowerCase(osgDB::getFileExtension(filename));
		if (fileexten == "osgb")
		{
			std::string simpleName = osgDB::getSimpleFileName(filename);
			std::string simpleNameWithoutExt = osgDB::getNameLessExtension(simpleName);

			if (!isCanProcess(simpleNameWithoutExt, _level))
			{
				return false;
			}

			osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
			if (!node.valid()) return false;

			std::vector<std::string> out_elements;
			osgDB::getPathElements(filename, out_elements);
			std::string vname = out_elements.empty() ? "default_name" : out_elements.back();
			/*node->setName(osgDB::getNameLessExtension(vname));*/
			GeodeVisitor gv(vname);
			node->accept(gv);

			root->addChild(node);
			return true;
		}
		return false;
	}

	bool isCanProcess(const std::string& simpleNameWithoutExt, int lodLevel)
	{
		gwUtil::StringVector outs;
		gwUtil::StringTokenizer stk(simpleNameWithoutExt, outs, "_");

		for (unsigned int i = 0; i < outs.size(); ++i)
		{
			std::string stringPart = osgDB::convertToLowerCase(outs[i]);
			if (gwUtil::startsWith(stringPart, "l"))
			{
				std::string levelStr = stringPart.substr(1, stringPart.size());
				int llevel = gwUtil::as(levelStr, -1);
				if (llevel == lodLevel) return true;
				else return false;
			}
		}
		return false;
	}

	bool handleDir(const std::string& path)
	{
		return true;
	}

private:
	int _level;
	std::string _inPath;
	std::string _outPath;
	std::string _ext;

	int _dirNum;
	int _maxDirNum;

	int _groupNum;

	osg::ref_ptr<osg::Group> _root;
};

//"E:/DATA/qxmx/qxmx/osgb,E:/DATA/qxmx/qxmx/test,3ds,23"
//"H:/Tile_1321013303332233120/osgb,H:/Tile_1321013303332233120/test, 3ds, 23"
//"E:/DATA/qxmx/qxmx/Tile_132102312010120213/Tile_132102312010120213/,E:/DATA/qxmx/qxmx/Tile_132102312010120213/test,3ds,18,1"
int main(int argc, char ** argv)
{
	osg::ArgumentParser arguments(&argc, argv);

	std::string parasStr;// = argv[3];
	arguments.read("-f", parasStr);
	gwUtil::StringVector paras;
	gwUtil::StringTokenizer stk(parasStr, paras, ",");

	if (paras.size() != 5)
	{
		MessageBox(NULL, "参数的数量不对, 请检查输入的参数", "错误", 1);
		return 0;
	}

	std::string inPath = paras[0];
	std::string outPath = paras[1];
	std::string outExt = gwUtil::replaceIn(paras[2], " ", "");
	int lodLevel = gwUtil::as(gwUtil::replaceIn(paras[3], " ", ""), -1);
	int maxDirNum = gwUtil::as(gwUtil::replaceIn(paras[4], " ", ""), 1);

	DirectoryVisitor dv(inPath, outPath, outExt, lodLevel, maxDirNum);
	//DirectoryVisitor dv("E:/DATA/qxmx/qxmx/osgb", "E:/DATA/qxmx/qxmx/test", "obj", 22, 1);

	return 0;
}
