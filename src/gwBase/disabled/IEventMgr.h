#ifndef IGWBASE_EVENTMGR_H
#define IGWBASE_EVENTMGR_H 1

#include <gwBase/ISubsystemMgr.h>
#include <gwBase/Callback.h>
#include <osg/ref_ptr>
#include <string>

class GWPropertyNode;

namespace gwBase
{
	class IEventMgr : public ISubsystem
	{
	public:
		IEventMgr() {}
		virtual ~IEventMgr() {}

		virtual void setRealtimeProperty(GWPropertyNode* node) = 0;

		/*** Add a single function callback event as a repeating task.
		* ex: addTask("foo", &Function ... )*/
		template<typename FUNC>
		inline void addTask(const std::string& name, const FUNC& f, double interval, double delay = 0, bool sim = false)
		{
			add(name, make_callback(f), interval, delay, true, sim);
		}

		/*** Add a single function callback event as a one-shot event.
		* ex: addEvent("foo", &Function ... )*/
		template<typename FUNC>
		inline void addEvent(const std::string& name, const FUNC& f, double delay, bool sim = false)
		{
			add(name, make_callback(f), 0, delay, false, sim);
		}

		/*** Add a object/method pair as a repeating task.
		* ex: addTask("foo", &object, &ClassName::Method, ...)*/
		template<class OBJ, typename METHOD>
		inline void addTask(const std::string& name, const OBJ& o, METHOD m, double interval, double delay = 0, bool sim = false)
		{
			add(name, make_callback(o, m), interval, delay, true, sim);
		}

		/*** Add a object/method pair as a repeating task.
		* ex: addEvent("foo", &object, &ClassName::Method, ...)*/
		template<class OBJ, typename METHOD>
		inline void addEvent(const std::string& name, const OBJ& o, METHOD m, double delay, bool sim = false)
		{
			add(name, make_callback(o, m), 0, delay, false, sim);
		}

		virtual void removeTask(const std::string& name) = 0;
	};

	typedef osg::ref_ptr<IEventMgr> IeventMgrPtr;
}

#endif // IGWBASE_EVENTMGR_H

