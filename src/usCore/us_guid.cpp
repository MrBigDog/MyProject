///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_guid.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : guid class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_guid.h"

namespace uniscope_globe
{
	guid::guid( void )
	{

	}

	guid::~guid( void )
	{

	}

	ustring guid::create( void )
	{
		GUID guid;   
		HRESULT hr = CoCreateGuid(&guid);
		RPC_WSTR strGUID = NULL;
		UuidToString( &guid, &strGUID );
		ustring ret_val = (LPCTSTR)strGUID;
		RpcStringFree( &strGUID );
		return ret_val;
	}

	GUID guid::create( cpstr str_guid )
	{
		GUID guid;
		UuidFromString( (RPC_WSTR)str_guid , &guid );
		return guid;
	}

	ustring guid::create( GUID& in_guid )
	{
		RPC_WSTR strGUID = NULL;
		UuidToString( &in_guid, &strGUID );
		ustring ret_val = (LPCTSTR)strGUID;
		RpcStringFree( &strGUID );
		return ret_val;
	}

	void guid::guid_string_to_int64( cpstr str_guid, __int64& out_first, __int64& out_second )
	{
		GUID v_guid = guid::create(str_guid);
		GUID_TO_INT64( v_guid, out_first, out_second );
	}

	
}