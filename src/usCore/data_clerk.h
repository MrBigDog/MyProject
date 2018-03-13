/** @file 底层数据缓存系统接口
 *  @author Felix
 *  @date 2018/01/11
 * @ todo: 为支持更新，结合状态，配置临时数据删除机制
 */
#ifndef data_clerk_h__
#define data_clerk_h__

#include <usCore\Export.h>
#include "data_bank.h"
#include <usUtil/us_singleton.h>
#include <map>

namespace uniscope_globe
{
	/**@brief 数据状态，分永久性数据、临时数据、进程数据、机器数据、业务流程数据、角色数据、用户数据等等*/
	enum data_status
	{
		FOREVER = 0,
		TEMP,
		PROCESS,
		BUSINESS,
		ROLE,
		USER
	};
	/**db系统接口类,允许应用设置多个系统实例，默认状态下使用一个*/
	class USCORE_EXPORT data_clerk
	{
	public:
		typedef std::map< ustring, data_bank* > DataBankMap;
		data_clerk(void);
		~data_clerk(void);

	public:
		///创建缓存实例
		void get_or_create_bank_instance(const ustring& path);

		bool attach_entry(const std::string& key, const leveldb::Slice& value);

		bool query_entry_value(const std::string& key, std::string& value);

		bool delete_entry(const std::string& key);

	protected:
		void final();

	private:
		data_bank* m_databank;
	};


	class USCORE_EXPORT singleton_data_clerk : public singleton<data_clerk>
	{
	public:
		singleton_data_clerk() {}
		virtual ~singleton_data_clerk() {}
	};
}

#endif // data_clerk_h__

