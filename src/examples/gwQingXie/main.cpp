#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include "FileUtils.h"
#include "StringUtils.h"

#include <windows.h>

#include <osg/PagedLOD>
#include "st_tree.h"

class PagedLodVisitor : public osg::NodeVisitor
{
public:
	PagedLodVisitor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}

	void apply(osg::PagedLOD& pl)
	{
		for (unsigned int i = 0; i < pl.getNumFileNames(); ++i)
		{
			std::string filename = pl.getFileName(i);
			if (filename.empty()) continue;
			_filenames.push_back(filename);
		}
	}
	std::vector<std::string> _filenames;
};

void CreatTileTree(st_tree::tree<std::string>::node_type& tree_node, const std::string& tile_path/*, const std::string& tile_name*/)
{
	std::string fullname = tree_node.data();// osgDB::concatPaths(tile_path, tile_name);
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fullname);
	if (!node) return;

	PagedLodVisitor plv;
	node->accept(plv);

	if (plv._filenames.empty()) return;

	for (unsigned int i = 0; i < plv._filenames.size(); ++i)
	{
		std::string filename = plv._filenames.at(i);
		if (filename.empty()) continue;

		st_tree::tree<std::string>::node_type::iterator it
			= tree_node.insert(osgDB::concatPaths(tile_path, filename));

		CreatTileTree(*it, tile_path);
	}
}

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
		if (!osgDB::fileExists(filename))
		{
			gwUtil::makeDirectoryForFile(filename);
		}
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
		, _ext(ext)
	{
		if (!gwUtil::startsWith(_ext, "."))
		{
			_ext = "." + _ext;
		}

		if (osgDB::fileType(inPath) == osgDB::DIRECTORY)
		{
			traverseDirectory(inPath);
		}
	}

private:
	void DirectoryVisitor::traverseDirectory(const std::string& path)
	{
		if (osgDB::fileType(path) == osgDB::DIRECTORY)
		{
			if (!handleDir(path)) return;

			osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
			for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
			{
				if (f->compare(".") == 0 || f->compare("..") == 0)
					continue;

				std::string filepath = osgDB::concatPaths(path, *f);
				std::string fullname = osgDB::concatPaths(filepath, *f + ".osgb");

				//写出最粗糙级别sm1
				std::string sm1path = osgDB::concatPaths(_outPath, "sm1");
				writeToLocal(fullname, sm1path, _ext);

				st_tree::tree<std::string> tree;
				tree.insert(fullname);
				CreatTileTree(tree.root(), filepath);

				//遍历树，写出相应模型;
				unsigned int leafnum = 0;
				for (st_tree::tree<std::string>::df_pre_iterator j(tree.df_pre_begin()); j != tree.df_pre_end(); ++j)
				{
					if (j->depth() == 3)
					{
						//第二级sm;
						std::string smpath = osgDB::concatPaths(_outPath, "sm");
						writeToLocal(j->data(), smpath, _ext);
					}
					else if (j->depth() == 1)
					{
						//写出到最高级别cm;
						std::string cmpath = osgDB::concatPaths(_outPath, "cm");
						writeToLocal(j->data(), cmpath, _ext);
					}
				}
				tree.clear();
			}
		}
	}

	bool handleDir(const std::string& path)
	{
		return true;
	}

private:
	void writeToLocal(const std::string& filename, const std::string& outpath, const std::string& ext)
	{
		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
		if (!node.valid()) return;

		std::string stripname = osgDB::getStrippedName(filename);

		std::vector<std::string> out_elements;
		osgDB::getPathElements(filename, out_elements);

		std::string newPath = osgDB::concatPaths(outpath, out_elements[out_elements.size() - 2]);
		std::string newName = osgDB::concatPaths(newPath, stripname + ext);
		if (!osgDB::fileExists(newName))
		{
			gwUtil::makeDirectoryForFile(newName);
		}
		osgDB::writeNodeFile(*node, newName);
		if (ext == ".obj" || ext == "obj")
		{
			TextureAndImageVisitor tv(newPath);
			node->accept(tv);
		}
	}

private:
	std::string _inPath;
	std::string _outPath;
	std::string _ext;
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

	if (paras.size() < 3)
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
