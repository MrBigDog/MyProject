///////////////////////////////////////////////////////////
//  exception_base.h
//  Implementation of the Class exception_base
//  Created on:      09-十一月-2015 11:26:52
//  Original author: felix
///////////////////////////////////////////////////////////

#if !defined(EA_C45CEAF6_4991_403b_B176_96FFAF472782__INCLUDED_)
#define EA_C45CEAF6_4991_403b_B176_96FFAF472782__INCLUDED_
#include <usCore\Export.h>
#include <string>
#include <exception>


#define GW_THROW( ExClass, args, ... )									\																	\
	ExClass e( args );													\
	e.init( __FILE__, __FUNCTION__, __LINE__ );							\
	throw e;															

#define GW_DEFINE_EXCEPTION(ExClass, Base)								\
	ExClass(const std::string& msg = "") throw()						\
	:Base(msg)															\
	{}																	\
																		\
	~ExClass() throw(){}												\
																		\
	std::string get_class_name() const									\
	{																	\
	return #ExClass;													\
	}																	

#define DECLARE_EXCEPTION_CLASS(ExClass, Base)							\
	namespace gw_exception												\
	{																	\
class ExClass : public Base												\
		{																\
		public:															\
		ExClass(const std::string& msg = "") throw()					\
		:Base(msg)														\
			{}															\
																		\
			~ExClass() throw(){}										\
																		\
			std::string get_class_name() const							\
			{															\
			return #ExClass;											\
			}															\
		};																\
	}																								

#define DECLARE_EXCEPTION_CLASS_EX(ExClass)							\
	namespace gw_exception												\
	{																	\
class ExClass : public exception_base												\
		{																\
		public:															\
		ExClass(const std::string& msg = "") throw()					\
		:exception_base(msg)														\
			{}															\
																		\
			~ExClass() throw(){}										\
																		\
			std::string get_class_name() const							\
			{															\
			return #ExClass;											\
			}															\
		};																\
	}					

#ifdef _DEBUG///<mem定位
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

namespace gw_exception
{
	struct gw_memerory_leak_check
	{
		static void init() {
			//	检查内存泄露。
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}
	};

	/**
	 * 异常的基类，所有的子类异常从这个类继承
	 */
	class USCORE_EXPORT exception_base : public std::exception
	{
	public:
		exception_base(const std::string& msg = "") throw();
		virtual ~exception_base() throw();

		void init(const char* file, const char* func, int line);

		virtual std::string get_class_name() const;
		virtual std::string get_error_message() const;
		std::string get_stack_trace() const;

		const std::string& to_string() const;
		const char* what() const throw();

	private:


	protected:
		std::string msg_;
		const char* current_file_;
		const char* current_func_;
		int current_line_;

	private:
		mutable std::string what_;
	};

}

//DECLARE_EXCEPTION_CLASS(myexception,exception_base)///<使用方式

#endif // !defined(EA_C45CEAF6_4991_403b_B176_96FFAF472782__INCLUDED_)
