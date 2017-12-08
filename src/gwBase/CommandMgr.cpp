#include <gwBase/CommandMgr.h>
#include <gwBase/Exception.h>
#include <gwBase/Props.h>
#include <osg/OperationThread>
#include <osgViewer/ViewerBase>
#include <memory>
#include <cassert>

namespace gwBase
{
	namespace
	{
		class CommandOperation : public osg::Operation
		{
		public:
			CommandOperation(Command* command, const GWPropertyNode& arg)
				: _command(command)
				, _arg(arg)
			{}

			void operator()(osg::Object*)
			{
				if (!_command) return;
				(*_command)(_arg);
			}

		private:
			Command* _command;
			GWPropertyNode _arg;
		};
	}
	//////////////////////////////////////////////////////////////////////////

	static CommandMgr* static_instance = NULL;

	CommandMgr::CommandMgr()
	{
		assert(static_instance == NULL);
		static_instance = this;
	}

	CommandMgr::~CommandMgr()
	{
		assert(static_instance == this);
		static_instance = NULL;
	}

	CommandMgr* CommandMgr::instance()
	{
		return static_instance ? static_instance : new CommandMgr;
	}

	void CommandMgr::addCommandObject(const std::string &name, Command* command)
	{
		if (_commands.find(name) != _commands.end())
		{
			throw gw_exception("duplicate command name:" + name);
		}
		_commands[name] = command;
	}

	Command* CommandMgr::getCommand(const std::string &name) const
	{
		const command_map::const_iterator it = _commands.find(name);
		return (it != _commands.end() ? it->second : 0);
	}

	StringList CommandMgr::getCommandNames() const
	{
		StringList names;
		command_map::const_iterator it = _commands.begin();
		command_map::const_iterator last = _commands.end();
		while (it != last)
		{
			names.push_back(it->first);
			++it;
		}
		return names;
	}

	bool CommandMgr::execute(const std::string &name, const GWPropertyNode& arg, bool isExecuteAsOperation) const
	{
		Command* command = getCommand(name);
		if (command == 0)
		{
			return false;
		}
		if (!isExecuteAsOperation)
		{
			return (*command)(arg);
		}
		else
		{
			//如果OperationQueue队列不可用，则在主线程中执行;
			if (!_operationQueue.valid())
			{
				//GW_LOG()
				return (*command)(arg);
			}

			osg::ref_ptr<osg::OperationQueue> queue;
			if (_operationQueue.lock(queue))
			{
				queue->add(new CommandOperation(command, arg));
			}
			return true;
		}
	}

	bool CommandMgr::removeCommand(const std::string& name)
	{
		command_map::iterator it = _commands.find(name);
		if (it == _commands.end())
		{
			return false;
		}

		delete it->second;
		_commands.erase(it);
		return true;
	}

	void CommandMgr::getUpdateOperationQueue(osgViewer::ViewerBase * viewbase)
	{
		if (!viewbase) return;

		osg::OperationQueue* q = viewbase->getUpdateOperations();
		if (!q)
		{
			q = new osg::OperationQueue;
			viewbase->setUpdateOperations(q);
		}
		_operationQueue = q;
	}
}
