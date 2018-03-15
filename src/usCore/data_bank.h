/** @file 底层数据缓存系统
 *  @author Felix
 *  @date 2018/01/11
 */
#ifndef data_bank_h__
#define data_bank_h__

#include <usCore\Export.h>
#include <usUtil/us_common_file.h>

#include <leveldb/db.h>

namespace uniscope_globe
{
	class USCORE_EXPORT data_bank
	{
	private:
		explicit data_bank(const ustring& bank_path);
	public:
		~data_bank(void);
		friend data_bank* create_data_bank(const ustring& bank_path);

		bool create_data_system();

		bool attach_entry(const leveldb::Slice& key, const leveldb::Slice& value);

		bool query_entry_value(const leveldb::Slice& key, std::string& value);

		bool delete_entry(const leveldb::Slice& key);

	protected:
		void set_options_create_if_missing(bool flag);

		void close();

	private:
		leveldb::DB * m_bank;
		leveldb::Options m_op;
		ustring m_bank_address;
	};

	inline data_bank* create_data_bank(const ustring& bank_path)
	{
		data_bank* db = new data_bank(bank_path);
		if (true != db->create_data_system()) return NULL;

		return db;
	}
}

#endif // data_bank_h__
