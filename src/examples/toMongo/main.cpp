#pragma warning(disable: 4996)

#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>

#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/gridfs/bucket.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osg/ShapeDrawable>

//#ifdef _DEBUG
//#pragma comment(lib, "bsoncxxd.lib")
//#pragma comment(lib, "mongocxxd.lib")
//#pragma comment(lib, "osgd.lib")
//#pragma comment(lib, "osgDBd.lib")
//#else
//#pragma comment(lib, "bsoncxx.lib")
//#pragma comment(lib, "mongocxx.lib")
//#pragma comment(lib, "osg.lib")
//#pragma comment(lib, "osgDB.lib")
//#endif // _DEBUG

using namespace mongocxx;
using bsoncxx::stdx::make_unique;



class DirectoryVisitor
{
public:
	DirectoryVisitor(const mongocxx::database& db)
		:_db(db)
	{
		_bucket = db.gridfs_bucket();
	}

public:
	void traverse(const std::string& path)
	{
		if (osgDB::fileType(path) == osgDB::DIRECTORY)
		{
			if (handleDir(path))
			{
				osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
				for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
				{
					if (f->compare(".") == 0 || f->compare("..") == 0)
						continue;

					std::string filepath = osgDB::concatPaths(path, *f);
					traverse(filepath);
				}
			}
		}
		else if (osgDB::fileType(path) == osgDB::REGULAR_FILE)
		{
			handleFile(path);
		}
	}

private:
	void handleFile(const std::string& filename)
	{
		std::string fileExt = osgDB::getLowerCaseFileExtension(filename);
		if (fileExt != "osgb") return;

		std::string simplename = osgDB::getSimpleFileName(filename);

		gridfs::uploader uploader = _bucket.open_upload_stream(osgDB::getNameLessExtension(simplename));
		std::ifstream file1(filename, std::ios_base::in | std::ios_base::binary);
		std::uint8_t ch = file1.get();
		std::vector<std::uint8_t> bytes;
		while (file1.good())
		{
			bytes.push_back(ch);
			ch = file1.get();
		}
		uploader.write(&bytes[0], bytes.size());
		bytes.clear();
		file1.close();
		auto result = uploader.close();
	}

	bool handleDir(const std::string& path)
	{
		return true;
	}

private:
	gridfs::bucket _bucket;
	mongocxx::database _db;
};

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

int main()
{
	mongocxx::instance inst{};
	mongocxx::uri uri("mongodb://192.168.1.154:27017");
	mongocxx::client conn{ uri };
	mongocxx::database db = conn["wjh_model"];

	DirectoryVisitor dv(db);
	dv.traverse("D:/Data/LFSJ_OSGB_NZ");
	//auto bucket = db.gridfs_bucket();

	////"sample_gridfs_file" is the name of the GridFS file stored on the server. GridFS filenames are not unique.
	//auto uploader = bucket.open_upload_stream("sample_gridfs_file");

	//std::ifstream file1("0.ive", std::ios_base::in | std::ios_base::binary);
	//std::uint8_t ch = file1.get();
	//std::vector<std::uint8_t> bytes;
	//while (file1.good())
	//{
	//	bytes.push_back(ch);
	//	ch = file1.get();
	//}
	//uploader.write(&bytes[0], bytes.size());
	//bytes.clear();
	//file1.close();

	//auto result = uploader.close();


	//bsoncxx::document::value doc_value = make_document(kvp("filename", "1cd88030-3a69-483b-9427-0325da4ee54f"));

	//mongocxx::collection coll = db.collection("fs.files");
	////bsoncxx::document::view docview = coll.find_one(bsoncxx::document::view_or_value(doc_value)).value();
	//auto cursor = coll.find(bsoncxx::document::view_or_value(doc_value));
	//bsoncxx::document::element id_ele = /*docview*/(*cursor.begin())["_id"];

	//if (!id_ele.raw())
	//{
	//	return 0;
	//}

	//bsoncxx::types::value id = id_ele.get_value();

	//auto downloader = bucket.open_download_stream(id);
	//auto file_length = downloader.file_length();
	//auto bytes_counter = 0;

	//auto buffer_size = std::min(file_length, static_cast<std::int64_t>(downloader.chunk_size()));
	//auto buffer = make_unique<std::uint8_t[]>(static_cast<std::size_t>(buffer_size));

	//std::basic_ofstream<std::uint8_t> outfile("test.osgb", std::ofstream::binary);
	//while (auto length_read = downloader.read(buffer.get(), static_cast<std::size_t>(buffer_size)))
	//{
	//	bytes_counter += static_cast<std::int32_t>(length_read);
	//	outfile.write(buffer.get(), bytes_counter);
	//}

	//std::cout << "total bytes in file: " << bytes_counter << std::endl;

	system("pause");
}
