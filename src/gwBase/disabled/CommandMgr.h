#ifndef commands_h__
#define commands_h__

#include "interface/ICommandMgr.h"
#include "interface/IApplication.h"
#include <map>

class CommandMgr : public ICommandMgr
{
public:
	explicit CommandMgr(IApplication* app);
	virtual ~CommandMgr();

	virtual void addCommandObject(const std::string &name, ICommand* command);

	virtual ICommand* getCommand(const std::string &name) const;

	virtual string_list getCommandNames() const;

	virtual bool execute(const std::string &name, void * arg) const;

	virtual bool removeCommand(const std::string& name);

private:
	typedef std::map<std::string, ICommand*> command_map;
	command_map commands_;
	IApplication* _app;
};


#endif // commands_h__
