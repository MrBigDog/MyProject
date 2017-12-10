#ifndef GWAPP_IENVIRONMENT_H
#define GWAPP_IENVIRONMENT_H 1

#include <string>
#include <vector>

class IEnvironment 
{
public:
	IEnvironment() {}
	virtual ~IEnvironment(){}

	virtual void InitFilter() = 0;

	virtual void GetAllPluginsFromConfig( std::vector<std::string>& plugins ) = 0;

	virtual void SetModuleName( std::string& name ) = 0;

	virtual std::string GetModuleName() const = 0;

	virtual void SetScaleBarImagePath( std::string& img ) = 0;

	virtual std::string GetScaleBarImagePath() const = 0;

	virtual void SetGeoModelPath( std::string& path ) = 0;

	virtual std::string GetGeoModelPath() const = 0;
};


#endif //GWAPP_IENVIRONMENT_H





