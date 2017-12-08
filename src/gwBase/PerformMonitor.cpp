#include <gwBase/PerformMonitor.h>
#include <gwBase/SmpleStatistic.h>
#include <gwBase/LogStream.h>
#include <gwBase/Exception.h>

#include <stdio.h>
#include <string.h>
#include <string>

namespace gwBase
{
	PerformanceMonitor::PerformanceMonitor(SubsystemMgr* subSysMgr, GWPropertyNodePtr root)
		: _isEnabled(false), _count(0)
	{
		_root = root;
		_subSysMgr = subSysMgr;
	}

	void PerformanceMonitor::bind(void)
	{
		_statisticsFlag = _root->getChild("enabled", 0, true);
		_statisticsInterval = _root->getChild("interval-s", 0, true);
		_statiticsSubsystems = _root->getChild("subsystems", 0, true);
	}

	void PerformanceMonitor::unbind(void)
	{
		_statiticsSubsystems = 0;
		_statisticsFlag = 0;
		_statisticsInterval = 0;
	}

	void PerformanceMonitor::init(void)
	{ }

	void PerformanceMonitor::update(double dt)
	{
		if (_isEnabled != _statisticsFlag->getBoolValue())
		{
			// flag has changed, update subsystem manager
			_isEnabled = _statisticsFlag->getBoolValue();
			if (_isEnabled)
			{
				_subSysMgr->setReportTimingCb(this, &subSystemMgrHook);
				//_lastUpdate.stamp();
				_lastUpdate.setStartTick();
			}
			else
			{
				_subSysMgr->setReportTimingCb(this, 0);
			}
		}

		if (!_isEnabled)
		{
			return;
		}
		if (/*_lastUpdate.elapsedMSec()*/_lastUpdate.time_m() > 1000 * _statisticsInterval->getDoubleValue())
		{
			_count = 0;
			// grab timing statistics
			_subSysMgr->reportTiming();
			//_lastUpdate.stamp();
			_lastUpdate.setStartTick();
		}
	}

	/** Callback hooked into the subsystem manager. */
	void PerformanceMonitor::subSystemMgrHook(void* userData, const std::string& name, gwBase::SampleStatistic* timeStat)
	{
		((PerformanceMonitor*)userData)->reportTiming(name, timeStat);
	}

	/** Grabs and exposes timing information to properties */
	void PerformanceMonitor::reportTiming(const string& name, gwBase::SampleStatistic* timeStat)
	{
		GWPropertyNode* node = _statiticsSubsystems->getChild("subsystem", _count++, true);
		if (!node) return;

		double minMs = timeStat->min() / 1000;
		double maxMs = timeStat->max() / 1000;
		double meanMs = timeStat->mean() / 1000;
		double stdDevMs = timeStat->stdDev() / 1000;
		double totalMs = timeStat->total() / 1000;
		double cumulativeMs = timeStat->cumulative() / 1000;
		int    samples = timeStat->samples();

		node->setStringValue("name", name);
		node->setDoubleValue("min-ms", minMs);
		node->setDoubleValue("max-ms", maxMs);
		node->setDoubleValue("mean-ms", meanMs);
		node->setDoubleValue("stddev-ms", stdDevMs);
		node->setDoubleValue("total-ms", totalMs);
		node->setDoubleValue("cumulative-ms", cumulativeMs);
		node->setDoubleValue("count", samples);

		timeStat->reset();
	}
}