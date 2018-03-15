/** @file 底层数据缓存系统具体实现
 *  @author Felix
 *  @date 2018/01/11
 */
 //#include "StdAfx.h"
#include "data_bank.h"
#include <usUtil/us_string_ext.h>

using namespace uniscope_globe;

data_bank::data_bank(const ustring& bank_path)
{
	m_bank_address = bank_path;
}


data_bank::~data_bank(void)
{
	close();
}

bool data_bank::create_data_system()
{
	set_options_create_if_missing(true);

	leveldb::Status s = leveldb::DB::Open(m_op, string_ext::from_wstring(m_bank_address), &m_bank);

	if (!s.ok()) return false;

	return true;
}

bool data_bank::attach_entry(const leveldb::Slice& key, const leveldb::Slice& value)
{
	leveldb::Status s = m_bank->Put(leveldb::WriteOptions(), key, value);
	if (!s.ok()) return false;

	return true;
}

bool data_bank::query_entry_value(const leveldb::Slice& key, std::string& value)
{
	leveldb::Status s = m_bank->Get(leveldb::ReadOptions(), key, &value);
	if (!s.ok()) return false;

	return true;
}

bool data_bank::delete_entry(const leveldb::Slice& key)
{
	leveldb::Status s = m_bank->Delete(leveldb::WriteOptions(), key);
	if (!s.ok()) return false;

	return true;
}

void data_bank::set_options_create_if_missing(bool flag)
{
	m_op.create_if_missing = flag;
}

void data_bank::close()
{
	delete m_bank;
}

