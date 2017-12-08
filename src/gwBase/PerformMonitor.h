#ifndef GWBASE_PERFMON_H
#define GWBASE_PERFMON_H 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/Props.h>
#include <osg/Timer>

namespace gwBase{class SampleStatistic;}

namespace gwBase
{
	class GW_BASE_EXPORT PerformanceMonitor : public Subsystem
	{
	public:
		PerformanceMonitor(SubsystemMgr* subSysMgr, GWPropertyNodePtr root);

		virtual void bind(void);
		virtual void unbind(void);
		virtual void init(void);
		virtual void update(double dt);

	private:
		static void subSystemMgrHook(void* userData, const std::string& name, gwBase::SampleStatistic* timeStat);

		void reportTiming(const std::string& name, gwBase::SampleStatistic* timeStat);

		osg::Timer _lastUpdate;
		SubsystemMgr* _subSysMgr;
		GWPropertyNodePtr _root;
		GWPropertyNodePtr _statiticsSubsystems;
		GWPropertyNodePtr _statisticsFlag;
		GWPropertyNodePtr _statisticsInterval;

		bool _isEnabled;
		int _count;
	};
}

#endif // GWBASE_PERFMON_H
