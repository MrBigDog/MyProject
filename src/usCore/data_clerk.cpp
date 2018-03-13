//#include "StdAfx.h"
#include "data_clerk.h"

using namespace uniscope_globe;

data_clerk* singleton_data_clerk::s_ins = NULL;

data_clerk::data_clerk(void)
{
	m_databank = NULL;
}


data_clerk::~data_clerk(void)
{
	/*m_databank = NULL;*/
	AUTO_DELETE( m_databank )
}

void data_clerk::get_or_create_bank_instance(const ustring& path )
{
	if ( path.empty() || NULL != m_databank) return;

	m_databank = create_data_bank( path );
}
bool data_clerk::attach_entry( const std::string& key, const leveldb::Slice& value )
{
	return m_databank->attach_entry( key, value );
}

bool data_clerk::query_entry_value( const std::string& key, std::string& value )
{
	return m_databank->query_entry_value( key, value );
}

bool data_clerk::delete_entry( const std::string& key )
{
	return m_databank->delete_entry( key );
}

void data_clerk::final()
{
	//AUTO_DELETE( m_databank );
}
