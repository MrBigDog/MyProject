#ifndef GWBASE_ICOMMANDMGR_H
#define GWBASE_ICOMMANDMGR_H 1

#include <gwUtil/TypesDef.h>

#include <osg/ref_ptr>
#include <osg/Referenced>
#include <vector>
#include <string>

namespace osgViewer { class ViewerBase; }

class GWPropertyNode;

namespace gwBase
{
	class Command
	{
	public:
		virtual ~Command() { }
		virtual bool operator()(const GWPropertyNode& arg) = 0;
	};
	typedef bool(*command_t) (const GWPropertyNode& arg);

	class ICommandMgr : public osg::Referenced
	{
	public:
		virtual ~ICommandMgr() {};

		virtual void addCommand(const std::string& name, command_t f) = 0;

		virtual void addCommandObject(const std::string &name, Command* command) = 0;

		template<class OBJ, typename METHOD>
		void addCommand(const std::string& name, const OBJ& o, METHOD m) = 0;

		virtual Command* getCommand(const std::string &name) const = 0;

		virtual StringList getCommandNames() const = 0;

		virtual bool execute(const std::string &name, const GWPropertyNode& arg, bool isSingleThreadExecute) const = 0;

		virtual bool removeCommand(const std::string& name) = 0;

		virtual void getUpdateOperationQueue(osgViewer::ViewerBase* viewbase) = 0;
	};

	typedef osg::ref_ptr<ICommandMgr> ICommandMgrPtr;
}

#endif // GWAPP_ICOMMANDMGR_H
