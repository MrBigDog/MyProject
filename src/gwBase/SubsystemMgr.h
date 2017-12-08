#ifndef GWBASE_SUBSYSTEMMGR_H
#define GWBASE_SUBSYSTEMMGR_H 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/TypesDef.h>
#include <osg/Timer>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <map>
#include <string>
#include <vector>

namespace gwBase { class SampleStatistic; }

namespace gwBase
{
	class TimingInfo
	{
	public:
		TimingInfo(const std::string& name, const osg::Timer_t &t)
			: eventName(name), time(t)
		{ }

		const std::string& getName() const { return eventName; }
		const osg::Timer_t& getTime() const { return time; }
	private:
		std::string eventName;
		osg::Timer_t time;
	};
	typedef std::vector<TimingInfo> eventTimeVec;

	typedef void(*SubsystemTimingCb)(void* userData, const std::string& name, gwBase::SampleStatistic* pStatistic);

	//
	class GW_BASE_EXPORT Subsystem : public osg::Referenced
	{
	public:
		typedef enum
		{
			INIT_DONE,
			INIT_CONTINUE
		} InitStatus;

		virtual ~Subsystem() {}

		const std::string getName() const { return _name; }
		void setName(const std::string& name) { _name = name; }

		virtual void init() {}

		virtual InitStatus incrementalInit()
		{
			init();
			return INIT_DONE;
		}

		virtual void postinit() {}
		virtual void reinit() {}

		virtual void shutdown() {}
		virtual void bind() {}
		virtual void unbind() {}

		virtual void update(double delta_time_sec) = 0;

		virtual void suspend() { _suspended = true; }
		virtual void suspend(bool suspended) { _suspended = suspended; }

		virtual void resume() { _suspended = false; }

		virtual bool is_suspended() const { return _suspended; }

		virtual void reportTiming(void) {}

		virtual void stamp(const std::string& name)
		{
			timingInfo.push_back(TimingInfo(name, osg::Timer::instance()->tick()));
		}

	protected:
		eventTimeVec timingInfo;
		static SubsystemTimingCb reportTimingCb;
		static void* reportTimingUserData;
		bool _suspended;
		std::string _name;
	};
	typedef osg::ref_ptr<Subsystem> SubsystemPtr;

	//
	class GW_BASE_EXPORT SubsystemGroup : public Subsystem
	{
	public:
		SubsystemGroup();
		virtual ~SubsystemGroup();

		virtual void init();
		virtual InitStatus incrementalInit();
		virtual void postinit();
		virtual void reinit();
		virtual void shutdown();
		virtual void bind();
		virtual void unbind();
		virtual void update(double delta_time_sec);
		virtual void suspend();
		virtual void resume();
		virtual bool is_suspended() const;

		virtual void set_subsystem(const std::string &name, Subsystem * subsystem, double min_step_sec = 0);
		virtual Subsystem * get_subsystem(const std::string &name);
		virtual void remove_subsystem(const std::string &name);
		virtual bool has_subsystem(const std::string &name) const;

		template<class T>
		T* getSubsystem(const std::string &name) const
		{
			return dynamic_cast<T*>(get_subsystem(name));
		}

		virtual void clearSubsystems();

		void reportTiming(void);

		void set_fixed_update_time(double fixed_dt);

		StringList member_names() const;

	private:
		class Member;
		Member* get_member(const std::string &name, bool create = false);

		typedef std::vector<Member *> MemberVec;
		MemberVec _members;

		double _fixedUpdateTime;
		double _updateTimeRemainder;

		/// index of the member we are currently init-ing
		unsigned int _initPosition;
	};
	typedef osg::ref_ptr<SubsystemGroup> SubsystemGroupRef;

	class GW_BASE_EXPORT SubsystemMgr : public Subsystem
	{
	public:
		enum GroupType
		{
			INIT = 0,
			GENERAL,
			EARTH,
			//DISPLAY,    ///< view, camera, rendering updates
			//SOUND,
			MAX_GROUPS
		};

	public:
		SubsystemMgr();
		virtual ~SubsystemMgr();

		virtual void init();
		virtual InitStatus incrementalInit();
		virtual void postinit();
		virtual void reinit();
		virtual void shutdown();
		virtual void bind();
		virtual void unbind();
		virtual void update(double delta_time_sec);
		virtual void suspend();
		virtual void suspend(bool suspended);
		virtual void resume();
		virtual bool is_suspended() const;
		virtual void stamp(const std::string& name) {}

		virtual void add(const char * name, Subsystem * subsystem, GroupType group/* = GENERAL*/, double min_time_sec = 0);

		/**
		 * remove a subsystem, and return a pointer to it.
		 * returns NULL if the subsystem was not found.
		 */
		virtual void remove(const char* name);

		virtual SubsystemGroup * get_group(GroupType group);

		virtual Subsystem* get_subsystem(const std::string &name) const;

		void reportTiming();
		void setReportTimingCb(void* userData, SubsystemTimingCb cb)
		{
			reportTimingCb = cb;
			reportTimingUserData = userData;
		}

		template<class T>
		T* get_subsystem() const
		{
			return dynamic_cast<T*>(get_subsystem(T::subsystemName()));
		}

	private:
		std::vector<SubsystemGroupRef> _groups;
		unsigned int _initPosition;

		// non-owning reference
		typedef std::map<std::string, SubsystemPtr > SubsystemDict;
		SubsystemDict _subsystem_map;
	};

	typedef osg::ref_ptr<SubsystemMgr> SubsystemMgrPtr;
}

#endif // GWBASE_SUBSYSTEMMGR_H
