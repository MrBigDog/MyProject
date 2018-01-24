#include <iostream>

#include <osgEarth/Config>
#include <osgEarth/URI>

int main(int argc, char** argv)
{
	std::string location = "E:\\DATA\\GeoData\\Workspace\\800\\Workspace\\model.xml";
	osgEarth::ReadResult r = osgEarth::URI(location).readString();
	if (r.failed())
	{
		return 1;
	}

	std::stringstream buf(r.getString());

	osgEarth::Config cf;
	cf.fromXML(buf);

	system("pause");

	return 0;
}
