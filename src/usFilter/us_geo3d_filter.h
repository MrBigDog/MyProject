///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEO3D_FILTER_H_
#define _US_GEO3D_FILTER_H_
#include <usFilter\Export.h>

namespace gmml
{
	class GeologicFeature;
	class GeoModel;
}
namespace se
{
	class GeoSymbolizer;
}

namespace uniscope_globe
{
	class USFILTER_EXPORT geo3d_filter
		: public filter_base
	{
	public: 
		geo3d_filter( void ) { }

		virtual ~geo3d_filter( void ) { }

	public:
		virtual object_base* parse( raw_buffer* in_buffer);
		static void parse(ustring strXmlPath, ustring strLayer, document_base* doc,std::list<object_base*>& lstObjs);

		static void construct_single_mesh(document_base* in_doc, d3d9_hardware_mesh* in_hardware_mesh, gmml::GeologicFeature* pFC, se::GeoSymbolizer* gs);

		static void readModelGeoData(gmml::GeoModel* pModel, std::map<std::string, std::map<std::string, gmml::GeologicFeature*>>& geoData);
		static void MoveWholeModelToZero(std::map<std::string, std::map<std::string, gmml::GeologicFeature*>>& geoData);
	};
}

#endif // _US_GEO3D_FILTER_H_