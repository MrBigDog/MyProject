#ifndef GWAPP_ICOMAND_H 
#define GWAPP_ICOMAND_H 1
#include <string>

namespace GeoApp
{
	///@brief 命令接口
	class ICommand
	{
	public:
		virtual ~ICommand() { }
		virtual bool operator()(void * arg) = 0;

		virtual ICommand* clone() const = 0;
		/*virtual void SetName( std::string& name ){ _name = name; }
		virtual std::string GetName(){ return _name; }
		private:
		std::string  _name;*/
	};

	typedef bool (*command_t) (const void * arg);///<函数指针定义

	///方法命令
	class FunctionCommand : public ICommand
	{
	public:
		FunctionCommand( command_t fun )
			: f_(fun)
		{}

		virtual bool operator()(void * arg) { return (*f_)(arg); }

		virtual ICommand* clone() const { return new FunctionCommand( *this ); }
	private:
		command_t f_;
	};

	///成员函数命令
	template< class ObjPtr, typename MemFn >
	class MethodCommand : public ICommand
	{
	public:
		MethodCommand( const ObjPtr& pObj, MemFn pMemFn ) 
			: pObj_(pObj)
			, pMemFn_(pMemFn)
		{}

		virtual bool operator()(void * arg)
		{
			return ((*pObj_).*pMemFn_)(arg);
		}

		virtual ICommand* clone() const{ return new MethodCommand( *this );}

	private:
		ObjPtr pObj_;
		MemFn pMemFn_;
	};

	template< class ObjPtr, typename MemFn >
	ICommand* make_functor( const ObjPtr& pObj, MemFn pMemFn )
	{
		return new MethodCommand<ObjPtr,MemFn>(pObj, pMemFn );
	}

	//---------------------------------------------------------------------------------
}
#endif //GWAPP_ICOMAND_H
