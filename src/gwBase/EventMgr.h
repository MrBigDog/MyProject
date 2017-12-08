#ifndef GWBASE_EVENTMGR_H
#define GWBASE_EVENTMGR_H 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/Props.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/Callback.h>
#include <string>

namespace gwBase
{
	class GW_BASE_EXPORT Timer
	{
	public:
		~Timer();
		void run();

		std::string name;
		double interval;
		Callback* callback;
		bool repeat;
		bool running;
	};

	class GW_BASE_EXPORT TimerQueue
	{
	public:
		TimerQueue(int preSize = 1);
		~TimerQueue();

		void clear();
		void update(double deltaSecs);

		double now() { return _now; }

		void   insert(Timer* timer, double time);
		Timer* remove(Timer* timer);
		Timer* remove();

		Timer* nextTimer() { return _numEntries ? _table[0].timer : 0; }
		double nextTime() { return -_table[0].pri; }

		Timer* findByName(const std::string& name) const;

	private:
		// The "priority" is stored as a negative time.  This allows the
		// implementation to treat the "top" of the heap as the largest
		// value and avoids developer mindbugs. ;)
		struct HeapEntry { double pri; Timer* timer; };

		int parent(int n) { return ((n + 1) / 2) - 1; }
		int lchild(int n) { return ((n + 1) * 2) - 1; }
		int rchild(int n) { return ((n + 1) * 2 + 1) - 1; }
		double pri(int n) { return _table[n].pri; }
		void swap(int a, int b)
		{
			HeapEntry tmp = _table[a];
			_table[a] = _table[b];
			_table[b] = tmp;
		}
		void siftDown(int n);
		void siftUp(int n);
		void growArray();

		// gcc complains there is no function specification anywhere.
		// void check();
		double _now;
		HeapEntry *_table;
		int _numEntries;
		int _tableSize;
	};

	class GW_BASE_EXPORT EventMgr : public Subsystem
	{
	public:
		EventMgr();
		~EventMgr();

		virtual void init();
		virtual void update(double delta_time_sec);
		virtual void unbind();
		virtual void shutdown();

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

		virtual void setRealtimeProperty(GWPropertyNode* node/*double realTimeProp*/) { _rtProp = node; }

		virtual void removeTask(const std::string& name);

	private:
		friend class Timer;
		void add(const std::string& name, Callback* cb, double interval, double delay, bool repeat, bool simtime);

		GWPropertyNodePtr /*double*/ _freezeProp;
		GWPropertyNodePtr /*double*/ _rtProp;
		TimerQueue _rtQueue;
		TimerQueue _simQueue;
		bool _inited;
	};

	typedef osg::ref_ptr<EventMgr> EventMgrPtr;
}

#endif // GWBASE_EVENTMGR_H

