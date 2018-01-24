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
		std::string imageName = image.getFileName();
		std::string sname = osgDB::getSimpleFileName(imageName);
		image.setFileName(sname);
		std::string filename = osgDB::concatPaths(_outpath, sname);
		if (!osgDB::fileExists(filename))
		{
			gwUtil::makeDirectoryForFile(filename);
			osgDB::writeImageFile(image, filename);
		}
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

		for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
		{
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
		//, _isCombine(true)
	{
		if (!gwUtil::startsWith(_ext, "."))
		{
			_ext = "." + _ext;
		}
		gwUtil::makeDirectory(_outPath);
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
			std::stringstream ss;
			ss << files.size() - 2;
			std::string filenumstr = ss.str();

			unsigned int fnum = 0;
			for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
			{
				if (f->compare(".") == 0 || f->compare("..") == 0)
					continue;

				fnum++;
				std::stringstream subss;
				subss << fnum;
				std::string inf = "总共" + filenumstr + "个文件夹，正在处理第" + subss.str() + "个......";
				std::cout << inf << std::endl;

				std::string filepath = osgDB::concatPaths(path, *f);
				std::string fullname = osgDB::concatPaths(filepath, *f + ".osgb");

				//写出最粗糙级别sm1
				std::string sm1path = osgDB::concatPaths(_outPath, "sm1");
				writeToLocal(fullname, sm1path, _ext);

				st_tree::tree<std::string> tree;
				tree.insert(fullname);
				CreatTileTree(tree.root(), filepath);

				//遍历树，写出相应模型;
				if (_ext == ".3ds")
				{
					osg::ref_ptr<osg::Group> cmRoot = new osg::Group;
					osg::ref_ptr<osg::Group> smRoot = new osg::Group;
					for (st_tree::tree<std::string>::df_pre_iterator j(tree.df_pre_begin()); j != tree.df_pre_end(); ++j)
					{
						if (j->depth() == 3)
						{
							//第二级sm;
							osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(j->data());
							if (node.valid()) smRoot->addChild(node);
						}
						else if (j->depth() == 1)
						{
							//写出到最高级别cm;
							osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(j->data());
							if (node.valid()) cmRoot->addChild(node);
						}
					}
					std::string smrelpath = osgDB::concatPaths("sm", *f);
					std::string smpath = osgDB::concatPaths(_outPath, smrelpath);
					//std::string smname = osgDB::concatPaths(smpath, *f + _ext);
					writeToLocal(smRoot, smpath, *f, _ext);

					std::string cmrelpath = osgDB::concatPaths("cm", *f);
					std::string cmpath = osgDB::concatPaths(_outPath, cmrelpath);
					//std::string cmname = osgDB::concatPaths(cmpath, *f + _ext);
					writeToLocal(cmRoot, cmpath, *f, _ext);
				}
				else if (_ext == ".obj")
				{
					std::string smpath = osgDB::concatPaths(_outPath, "sm");
					std::string cmpath = osgDB::concatPaths(_outPath, "cm");
					for (st_tree::tree<std::string>::df_pre_iterator j(tree.df_pre_begin()); j != tree.df_pre_end(); ++j)
					{
						if (j->depth() == 3)
						{
							//第二级sm;
							writeToLocal(j->data(), smpath, _ext);
						}
						else if (j->depth() == 1)
						{
							//写出到最高级别cm;
							writeToLocal(j->data(), cmpath, _ext);
						}
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
	void writeToLocal(osg::Node* node, const std::string& path, const std::string& name, const std::string& ext)
	{
		std::string filename = osgDB::concatPaths(path, name + ext);
		if (!osgDB::fileExists(filename))
		{
			gwUtil::makeDirectoryForFile(filename);
		}

		osg::ref_ptr<osgDB::Options> opts = 0L;
		if (ext == "3ds" || ext == ".3ds")
		{
			opts = new osgDB::Options("extended3dsFilePaths");
		}

		osgDB::writeNodeFile(*node, filename, opts);
		if (ext == ".obj" || ext == "obj")
		{
			TextureAndImageVisitor tv(path);
			node->accept(tv);
		}
	}

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
		osg::ref_ptr<osgDB::Options> opts = 0L;
		if (ext == "3ds" || ext == ".3ds")
		{
			//opts = new osgDB::Options("extended3dsFilePaths");
		}
		if (ext == ".obj" || ext == "obj")
		{
			TextureAndImageVisitor tv(newPath);
			node->accept(tv);
		}
		osgDB::writeNodeFile(*node, newName, opts);
	}

private:
	std::string _inPath;
	std::string _outPath;
	std::string _ext;
	//bool _isCombine;
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
	std::string outExt = osgDB::convertToLowerCase(paras[2]);
	int lodLevel = 0;
	int maxDirNum = 1;

	DirectoryVisitor dv(inPath, outPath, outExt, lodLevel, maxDirNum);

	//DirectoryVisitor dv("E:/DATA/qstt", "E:/DATA/qxmx/qxmx/test111/ttt/tt", "3ds", 22, 1);
	//system("pause");

	return 0;
}
