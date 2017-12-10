#ifndef GWBASE_ISUBSYSTEMMGR_H
#define GWBASE_ISUBSYSTEMMGR_H 1

#include <osg/Timer>
#include <osg/ref_ptr>
#include <osg/Referenced>

#include <string>
#include <vector>

namespace gwUtil { class SampleStatistic; }

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

	typedef void(*SubsystemTimingCb)(void* userData, const std::string& name, gwUtil::SampleStatistic* pStatistic);

	class ISubsystem : public osg::Referenced
	{
	public:
		typedef enum
		{
			INIT_DONE,
			INIT_CONTINUE
		} InitStatus;

		virtual ~ISubsystem() {}

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
	};
	typedef osg::ref_ptr<ISubsystem> ISubsystemPtr;

	class ISubsystemMgr :public ISubsystem
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

		virtual ~ISubsystemMgr() {}

		virtual void add(const char * name, ISubsystem * subsystem,
			GroupType group = GENERAL, double min_time_sec = 0) = 0;

		virtual void remove(const char* name) = 0;

		virtual ISubsystem* get_subsystem(const std::string &name) const = 0;

		template<class T>
		T* getSubsystem(const std::string &name) const
		{
			return dynamic_cast<T*>(get_subsystem(name));
		}
	};

	typedef osg::ref_ptr<ISubsystemMgr> ISubsystemMgrPtr;
}

#endif // GWAPP_ISUBSYSTEMMGR_H
