#include <memory>
#include <cassert>
#include "CommandMgr.h"
//#include "asyn_queue.h"

CommandMgr::CommandMgr(IApplication* app) :_app(app)
{ }

CommandMgr::~CommandMgr()
{
}

void CommandMgr::addCommandObject(const std::string &name, ICommand* command)
{
	if (commands_.find(name) != commands_.end()) return;///todo
		//throw gw_exception("duplicate command name:" + name);
	commands_[name] = command;
}

ICommand* CommandMgr::getCommand(const std::string &name) const
{
	const command_map::const_iterator it = commands_.find(name);
	return (it != commands_.end() ? it->second : 0);
}

string_list CommandMgr::getCommandNames() const
{
	string_list names;
	command_map::const_iterator it = commands_.begin();
	command_map::const_iterator last = commands_.end();
	while (it != last)
	{
		names.push_back(it->first);
		++it;
	}
	return names;
}

bool CommandMgr::execute(const std::string &name, void * arg) const
{
	ICommand* command = getCommand(name);
	if (command == 0)
	{
		return false;
	}
	try
	{
		//CommandNode cmd;
		//cmd._node_command = command->clone();
		//cmd._params = arg;
		//_app->GetTaskQueue()->push( cmd );
		return (*command)(arg);
	}
	catch (std::exception& ex)
	{
	}

	catch (...)
	{
	}

	return false;
}

bool CommandMgr::removeCommand(const std::string& name)
{
	command_map::iterator it = commands_.find(name);
	if (it == commands_.end())
		return false;

	delete it->second;
	commands_.erase(it);
	return true;
}

