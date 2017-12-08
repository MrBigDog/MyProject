#ifndef GWBASE_COMMANDMANAGER_H_ 
#define GWBASE_COMMANDMANAGER_H_ 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/Props.h>
#include <gwBase/TypesDef.h>

#include <osg/observer_ptr>

#include <map>
#include <vector>
#include <string>

namespace osg { class OperationQueue; }
namespace osgViewer { class ViewerBase; }

namespace gwBase
{
	class Command
	{
	public:
		virtual ~Command() { }
		virtual bool operator()(const GWPropertyNode& arg) = 0;
	};
	typedef bool(*command_t) (const GWPropertyNode& arg);

	class GW_BASE_EXPORT CommandMgr : public osg::Referenced
	{
	public:
		//class Command
		//{
		//public:
		//	virtual ~Command() { }
		//	virtual bool operator()(const GWPropertyNode& arg) = 0;
		//};
		//typedef bool(*command_t) (const GWPropertyNode& arg);

	private:
		class FunctionCommand : public Command
		{
		public:
			FunctionCommand(command_t fun)
				: f_(fun) {}

			virtual bool operator()(const GWPropertyNode& arg)
			{
				return (*f_)(arg);
			}

		private:
			command_t f_;
		};

		template< class ObjPtr, typename MemFn >
		class MethodCommand : public Command
		{
		public:
			MethodCommand(const ObjPtr& pObj, MemFn pMemFn)
				: pObj_(pObj)
				, pMemFn_(pMemFn)
			{}

			virtual bool operator()(const GWPropertyNode * arg)
			{
				return ((*pObj_).*pMemFn_)(arg);
			}

		private:
			ObjPtr pObj_;
			MemFn pMemFn_;
		};

		template< class ObjPtr, typename MemFn >
		Command* make_functor(const ObjPtr& pObj, MemFn pMemFn)
		{
			return new MethodCommand<ObjPtr, MemFn>(pObj, pMemFn);
		}

	public:
		CommandMgr();
		virtual ~CommandMgr();

		static CommandMgr* instance();

		void addCommand(const std::string& name, command_t f)
		{
			addCommandObject(name, new FunctionCommand(f));
		}

		void addCommandObject(const std::string &name, Command* command);

		template<class OBJ, typename METHOD>
		void addCommand(const std::string& name, const OBJ& o, METHOD m)
		{
			addCommandObject(name, make_functor(o, m));
		}

		virtual Command* getCommand(const std::string &name) const;

		virtual StringList getCommandNames() const;

		virtual bool execute(const std::string &name, const GWPropertyNode& arg, bool isExecuteAsOperation) const;

		bool removeCommand(const std::string& name);

		virtual void getUpdateOperationQueue(osgViewer::ViewerBase* viewbase);

	protected:

	private:
		typedef std::map<std::string, Command*> command_map;

		command_map _commands;

		osg::observer_ptr<osg::OperationQueue>  _operationQueue;
	};

	typedef osg::ref_ptr<CommandMgr> CommandMgrPtr;
}

#endif // CommandManager_h__
