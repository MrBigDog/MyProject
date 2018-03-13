/** @file �ײ����ݻ���ϵͳ�ӿ�
 *  @author Felix
 *  @date 2018/01/11
 * @ todo: Ϊ֧�ָ��£����״̬��������ʱ����ɾ������
 */
#ifndef data_clerk_h__
#define data_clerk_h__

#include <usCore\Export.h>
#include "data_bank.h"
#include <usUtil/us_singleton.h>
#include <map>

namespace uniscope_globe
{
	/**@brief ����״̬�������������ݡ���ʱ���ݡ��������ݡ��������ݡ�ҵ���������ݡ���ɫ���ݡ��û����ݵȵ�*/
	enum data_status
	{
		FOREVER = 0,
		TEMP,
		PROCESS,
		BUSINESS,
		ROLE,
		USER
	};
	/**dbϵͳ�ӿ���,����Ӧ�����ö��ϵͳʵ����Ĭ��״̬��ʹ��һ��*/
	class USCORE_EXPORT data_clerk
	{
	public:
		typedef std::map< ustring, data_bank* > DataBankMap;
		data_clerk(void);
		~data_clerk(void);

	public:
		///��������ʵ��
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

