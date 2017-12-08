#ifndef GWBASE_CALLBACKS_H
#define GWBASE_CALLBACKS_H 1

#include <compiler.h>

namespace gwBase
{
	class Callback
	{
	public:
		virtual ~Callback() {}
		virtual Callback* clone() const = 0;
		virtual void operator()() = 0;

	protected:
		Callback() {}

	private:
		void operator=(const Callback&);
	};

	/// * Callback for invoking a file scope function.
	template< typename Fun >
	class FunctionCallback : public Callback
	{
	public:
		FunctionCallback(const Fun& fun)
			: Callback(), f_(fun)
		{}

		Callback* clone() const
		{
			return new FunctionCallback(*this);
		}

		void operator()() { f_(); }

	private:
		FunctionCallback();

	private:
		Fun f_;
	};


	/*** Callback for invoking a member function.*/
	template< class ObjPtr, typename MemFn >
	class MethodCallback : public Callback
	{
	public:
		MethodCallback(const ObjPtr& pObj, MemFn pMemFn)
			: Callback()
			, pObj_(pObj)
			, pMemFn_(pMemFn)
		{ }

		Callback* clone() const
		{
			return new MethodCallback(*this);
		}

		void operator()()
		{
			((*pObj_).*pMemFn_)();
		}

	private:
		MethodCallback();

	private:
		ObjPtr pObj_;
		MemFn pMemFn_;
	};

	template< typename Fun >
	Callback* makeCallback(const Fun& fun)
	{
		return new FunctionCallback<Fun>(fun);
	}

	template< class ObjPtr, typename MemFn >
	Callback* makeCallback(const ObjPtr& pObj, MemFn pMemFn)
	{
		return new MethodCallback<ObjPtr, MemFn>(pObj, pMemFn);
	}
}

#endif // Callback_h__
