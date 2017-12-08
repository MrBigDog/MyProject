#include <gwBase/Math.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/SmpleStatistic.h>
#include <gwBase/Exception.h>
#include <gwBase/LogStream.h>

#include <osg/Timer>
#include <algorithm>

const int GW_MAX_SUBSYSTEM_EXCEPTIONS = 4;

namespace gwBase
{
	SubsystemTimingCb Subsystem::reportTimingCb = NULL;
	void* Subsystem::reportTimingUserData = NULL;

	////////////////////////////////////////////////////////////////////////
	// Implementation of SubsystemGroup.
	////////////////////////////////////////////////////////////////////////
	class GW_BASE_EXPORT SubsystemGroup::Member
	{
	private:
		Member(const Member &member);
	public:
		Member();
		virtual ~Member();

		virtual void update(double delta_time_sec);

		void reportTiming(void)
		{
			if (reportTimingCb)
			{
				reportTimingCb(reportTimingUserData, name, &timeStat);
			}
		}

		void updateExecutionTime(double time)
		{
			timeStat += time;
		}

		gwBase::SampleStatistic timeStat;
		std::string name;
		osg::ref_ptr<Subsystem> subsystem;
		double min_step_sec;
		double elapsed_sec;
		bool collectTimeStats;
		int exceptionCount;
		int initTime;
	};


	SubsystemGroup::SubsystemGroup()
		: _fixedUpdateTime(-1.0)
		, _updateTimeRemainder(0.0)
		, _initPosition(0)
	{ }

	SubsystemGroup::~SubsystemGroup()
	{
		// reverse order to prevent order dependency problems
		for (size_t i = _members.size(); i > 0; i--)
		{
			delete _members[i - 1];
		}
	}

	void SubsystemGroup::init()
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			_members[i]->subsystem->init();
		}
	}

	Subsystem::InitStatus SubsystemGroup::incrementalInit()
	{
		if (_initPosition >= _members.size())
		{
			return INIT_DONE;
		}
		//SGTimeStamp st;
		//st.stamp();

		osg::Timer timer;
		timer.setStartTick();
		InitStatus memberStatus = _members[_initPosition]->subsystem->incrementalInit();

		_members[_initPosition]->initTime += timer.time_m()/*st.elapsedMSec()*/;

		if (memberStatus == INIT_DONE)
		{
			++_initPosition;
		}
		return INIT_CONTINUE;
	}

	void SubsystemGroup::postinit()
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			_members[i]->subsystem->postinit();
		}
	}

	void SubsystemGroup::reinit()
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			_members[i]->subsystem->reinit();
		}
	}

	void SubsystemGroup::shutdown()
	{
		// reverse order to prevent order dependency problems
		for (size_t i = _members.size(); i > 0; i--)
		{
			_members[i - 1]->subsystem->shutdown();
		}
		_initPosition = 0;
	}

	void SubsystemGroup::bind()
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			_members[i]->subsystem->bind();
		}
	}

	void SubsystemGroup::unbind()
	{
		// reverse order to prevent order dependency problems
		for (size_t i = _members.size(); i > 0; i--)
		{
			_members[i - 1]->subsystem->unbind();
		}
	}

	void SubsystemGroup::update(double delta_time_sec)
	{
		int loopCount = 1;
		// if dt == 0.0, we are paused, so we need to run one iteration
		// of our members; if we have a fixed update time, we compute a
		// loop count, and locally adjust dt
		if ((delta_time_sec > 0.0) && (_fixedUpdateTime > 0.0))
		{
			double localDelta = delta_time_sec + _updateTimeRemainder;
			loopCount = gwBase::Miscd::roundToInt(localDelta / _fixedUpdateTime);
			loopCount = (std::max)(0, loopCount);
			_updateTimeRemainder = delta_time_sec - (loopCount * _fixedUpdateTime);
			delta_time_sec = _fixedUpdateTime;
		}

		bool recordTime = (reportTimingCb != NULL);

		//SGTimeStamp timeStamp;
		osg::Timer timer;
		while (loopCount-- > 0)
		{
			for (size_t i = 0; i < _members.size(); i++)
			{
				if (recordTime)
				{
					//timeStamp = SGTimeStamp::now();
					timer.setStartTick();
				}
				_members[i]->update(delta_time_sec); // indirect call

				if ((recordTime) && (reportTimingCb))
				{
					//timeStamp = SGTimeStamp::now() - timeStamp;
					_members[i]->updateExecutionTime(/*timeStamp.toUSecs()*/timer.time_s());
				}
			}
		} // of multiple update loop
	}

	void SubsystemGroup::reportTiming(void)
	{
		for (size_t i = _members.size(); i > 0; i--)
		{
			_members[i - 1]->reportTiming();
		}
	}

	void SubsystemGroup::suspend()
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			_members[i]->subsystem->suspend();
		}
	}

	void SubsystemGroup::resume()
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			_members[i]->subsystem->resume();
		}
	}

	StringList SubsystemGroup::member_names() const
	{
		StringList result;
		for (size_t i = 0; i < _members.size(); i++)
		{
			result.push_back(_members[i]->name);
		}

		return result;
	}

	bool SubsystemGroup::is_suspended() const
	{
		return false;
	}

	void SubsystemGroup::set_subsystem(const std::string &name, Subsystem * subsystem, double min_step_sec)
	{
		Member * member = get_member(name, true);
		member->name = name;
		member->subsystem = subsystem;
		member->min_step_sec = min_step_sec;
	}

	Subsystem * SubsystemGroup::get_subsystem(const std::string &name)
	{
		Member * member = get_member(name);
		if (member != 0)
		{
			return member->subsystem;
		}
		else
		{
			return 0;
		}
	}

	void SubsystemGroup::remove_subsystem(const std::string &name)
	{
		MemberVec::iterator it = _members.begin();
		for (; it != _members.end(); ++it)
		{
			if (name == (*it)->name)
			{
				delete *it;
				_members.erase(it);
				return;
			}
		}
		GW_LOG(GW_GENERAL, GW_WARN, "remove_subsystem: missing:" << name);
	}

	//------------------------------------------------------------------------------
	void SubsystemGroup::clearSubsystems()
	{
		for (MemberVec::iterator it = _members.begin(); it != _members.end(); ++it)
		{
			delete *it;
		}
		_members.clear();
	}

	void SubsystemGroup::set_fixed_update_time(double dt)
	{
		_fixedUpdateTime = dt;
	}

	bool SubsystemGroup::has_subsystem(const std::string &name) const
	{
		return (((SubsystemGroup *)this)->get_member(name) != 0);
	}

	SubsystemGroup::Member * SubsystemGroup::get_member(const std::string &name, bool create)
	{
		for (size_t i = 0; i < _members.size(); i++)
		{
			if (_members[i]->name == name)
			{
				return _members[i];
			}
		}
		if (create)
		{
			Member * member = new Member;
			_members.push_back(member);
			return member;
		}
		else
		{
			return 0;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Implementation of SubsystemGroup::Member
	////////////////////////////////////////////////////////////////////////

	SubsystemGroup::Member::Member()
		: name("")
		, subsystem(0)
		, min_step_sec(0)
		, elapsed_sec(0)
		, exceptionCount(0)
		, initTime(0)
	{
	}

	// This shouldn't be called due to subsystem pointer ownership issues.
	SubsystemGroup::Member::Member(const Member &)
	{}

	SubsystemGroup::Member::~Member()
	{}

	void SubsystemGroup::Member::update(double delta_time_sec)
	{
		elapsed_sec += delta_time_sec;
		if (elapsed_sec < min_step_sec)
		{
			return;
		}
		if (subsystem->is_suspended())
		{
			return;
		}

		try
		{
			subsystem->update(elapsed_sec);
			elapsed_sec = 0;
		}
		catch (gw_exception& e)
		{
			GW_LOG(GW_GENERAL, GW_ALERT, "caught exception processing subsystem:" << name << "\nmessage:" << e.getMessage());
			if (++exceptionCount > GW_MAX_SUBSYSTEM_EXCEPTIONS)
			{
				GW_LOG(GW_GENERAL, GW_ALERT, "(exceptionCount=" << exceptionCount << ", suspending)");
				subsystem->suspend();
			}
		}
	}


	////////////////////////////////////////////////////////////////////////
	// Implementation of SubsystemMgr.
	////////////////////////////////////////////////////////////////////////
	SubsystemMgr::SubsystemMgr()
		: _groups(MAX_GROUPS)
		, _initPosition(0)
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			//_groups[i].reset(new SubsystemGroup);
			_groups[i] = new SubsystemGroup;
		}
	}

	SubsystemMgr::~SubsystemMgr()
	{
		// ensure get_subsystem returns NULL from now onwards,
		// before the SubsystemGroup destructors are run
		_subsystem_map.clear();
		_groups.clear();
	}

	void SubsystemMgr::init()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->init();
		}
	}

	Subsystem::InitStatus SubsystemMgr::incrementalInit()
	{
		if (_initPosition >= MAX_GROUPS)
		{
			return INIT_DONE;
		}

		InitStatus memberStatus = _groups[_initPosition]->incrementalInit();
		if (memberStatus == INIT_DONE)
		{
			++_initPosition;
		}
		return INIT_CONTINUE;
	}

	void SubsystemMgr::postinit()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->postinit();
		}
	}

	void SubsystemMgr::reinit()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->reinit();
		}
	}

	void SubsystemMgr::shutdown()
	{
		// reverse order to prevent order dependency problems
		for (int i = MAX_GROUPS - 1; i >= 0; i--)
		{
			_groups[i]->shutdown();
		}
		_initPosition = 0;
	}


	void SubsystemMgr::bind()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->bind();
		}
	}

	void SubsystemMgr::unbind()
	{
		// reverse order to prevent order dependency problems
		for (int i = MAX_GROUPS - 1; i >= 0; i--)
		{
			_groups[i]->unbind();
		}
	}

	void SubsystemMgr::update(double delta_time_sec)
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->update(delta_time_sec);
		}
	}

	void SubsystemMgr::suspend()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->suspend();
		}
	}

	void SubsystemMgr::suspend(bool suspended)
	{
		_suspended = suspended;
	}

	void SubsystemMgr::resume()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->resume();
		}
	}

	bool SubsystemMgr::is_suspended() const
	{
		return false;
	}

	//不允许重名;
	void SubsystemMgr::add(const char * name, Subsystem * subsystem, GroupType group, double min_time_sec)
	{
		GW_LOG(GW_GENERAL, GW_DEBUG, "Adding subsystem " << name);
		get_group(group)->set_subsystem(name, subsystem, min_time_sec);

		if (_subsystem_map.find(name) != _subsystem_map.end())
		{
			GW_LOG(GW_GENERAL, GW_ALERT, "Adding duplicate subsystem " << name);
			throw gw_exception("duplicate subsystem:" + std::string(name));
		}
		_subsystem_map[name] = subsystem;
	}

	void SubsystemMgr::remove(const char* name)
	{
		SubsystemDict::iterator s = _subsystem_map.find(name);
		if (s == _subsystem_map.end())
		{
			return;
		}
		_subsystem_map.erase(s);

		// tedious part - we don't know which group the subsystem belongs too
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			if (_groups[i]->get_subsystem(name) != NULL)
			{
				_groups[i]->remove_subsystem(name);
				break;
			}
		} // of groups iteration
	}

	SubsystemGroup * SubsystemMgr::get_group(GroupType group)
	{
		return _groups[group];
	}

	Subsystem * SubsystemMgr::get_subsystem(const std::string &name) const
	{
		SubsystemDict::const_iterator s = _subsystem_map.find(name);
		if (s == _subsystem_map.end())
		{
			return 0;
		}
		else
		{
			return s->second;
		}
	}

	/** Trigger the timing callback to report data for all subsystems. */
	void SubsystemMgr::reportTiming()
	{
		for (int i = 0; i < MAX_GROUPS; i++)
		{
			_groups[i]->reportTiming();// of groups iteration
		}
	}
}