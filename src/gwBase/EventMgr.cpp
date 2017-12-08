#include <gwBase/EventMgr.h>

namespace gwBase
{
	void EventMgr::add(const std::string& name, Callback* cb, double interval, double delay, bool repeat, bool simtime)
	{
		// Clamp the delay value to 1 usec, so that user code can use
		// "zero" as a synonym for "next frame".
		if (delay <= 0) delay = 1e-6;
		if (interval <= 0) interval = 1e-6; // No timer endless loops please...

		Timer* t = new Timer;
		t->interval = interval;
		t->callback = cb;
		t->repeat = repeat;
		t->name = name;
		t->running = false;

		TimerQueue* q = simtime ? &_simQueue : &_rtQueue;
		q->insert(t, delay);
	}

	Timer::~Timer()
	{
		delete callback;
		callback = NULL;
	}

	void Timer::run()
	{
		(*callback)();
	}

	EventMgr::EventMgr() : _inited(false)
	{ }

	EventMgr::~EventMgr()
	{ }

	void EventMgr::unbind()
	{
		if (!_freezeProp->unref())
		{
			//delete _freezeProp;
			_freezeProp = 0;
		}
		if (!_rtProp->unref())
		{
			//delete _rtProp;
			_rtProp = 0;
		}
		//_freezeProp.clear();
		//_rtProp.clear();
	}

	void EventMgr::init()
	{
		if (_inited)
		{
			// protected against duplicate calls here, in case
			// init ever does something more complex in the future.
			return;
		}

		_inited = true;
	}

	void EventMgr::shutdown()
	{
		_inited = false;

		_simQueue.clear();
		_rtQueue.clear();
	}

	void EventMgr::update(double delta_time_sec)
	{
		_simQueue.update(delta_time_sec);

		double rt = _rtProp ? _rtProp->getDoubleValue() : 0;
		_rtQueue.update(rt);
	}

	void EventMgr::removeTask(const std::string& name)
	{
		// due to the ordering of the event-mgr in FG, tasks can be removed
		// after we are shutdown (and hence, have all been cleared). Guard
		// against this so we don't generate warnings below.
		if (!_inited)
		{
			return;
		}
		Timer* t = _simQueue.findByName(name);
		if (t)
		{
			_simQueue.remove(t);
		}
		else if ((t = _rtQueue.findByName(name)))
		{
			_rtQueue.remove(t);
		}
		else
		{
			//SG_LOG(GW_GENERAL, SG_WARN, "removeTask: no task found with name:" << name);
			return;
		}
		if (t->running)
		{
			// mark as not repeating so that the TimerQueue::update()
			// will clean it up
			t->repeat = false;
		}
		else
		{
			delete t;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// TimerQueue
	// This is the priority queue implementation:
	////////////////////////////////////////////////////////////////////////
	TimerQueue::TimerQueue(int size)
	{
		_now = 0;
		_numEntries = 0;
		_tableSize = 1;
		while (size > _tableSize)
		{
			_tableSize = ((_tableSize + 1) << 1) - 1;
		}

		_table = new HeapEntry[_tableSize];
		for (int i = 0; i < _tableSize; i++)
		{
			_table[i].pri = 0;
			_table[i].timer = 0;
		}
	}


	TimerQueue::~TimerQueue()
	{
		clear();
		delete[] _table;
	}

	void TimerQueue::clear()
	{
		// delete entries
		for (int i = 0; i < _numEntries; i++)
		{
			delete _table[i].timer;
		}

		_numEntries = 0;

		// clear entire table to empty
		for (int i = 0; i < _tableSize; i++)
		{
			_table[i].pri = 0;
			_table[i].timer = 0;
		}
	}

	void TimerQueue::update(double deltaSecs)
	{
		_now += deltaSecs;
		while (_numEntries && nextTime() <= _now)
		{
			Timer* t = remove();
			if (t->repeat)
			{
				insert(t, t->interval);
			}

			// warning: this is not thread safe
			// but the entire timer queue isn't either
			t->running = true;
			t->run();
			t->running = false;
			if (!t->repeat)
			{
				delete t;
			}
		}
	}

	void TimerQueue::insert(Timer* timer, double time)
	{
		if (_numEntries >= _tableSize)
		{
			growArray();
		}

		_numEntries++;
		_table[_numEntries - 1].pri = -(_now + time);
		_table[_numEntries - 1].timer = timer;

		siftUp(_numEntries - 1);
	}

	Timer* TimerQueue::remove(Timer* t)
	{
		int entry;
		for (entry = 0; entry < _numEntries; entry++)
		{
			if (_table[entry].timer == t) break;
		}

		if (entry == _numEntries)
		{
			return 0;
		}

		// Swap in the last item in the table, and sift down
		swap(entry, _numEntries - 1);
		_numEntries--;
		siftDown(entry);

		return t;
	}

	Timer* TimerQueue::remove()
	{
		if (_numEntries == 0)
		{
			return 0;
		}
		else if (_numEntries == 1)
		{
			_numEntries = 0;
			return _table[0].timer;
		}

		Timer *result = _table[0].timer;
		_table[0] = _table[_numEntries - 1];
		_numEntries--;
		siftDown(0);
		return result;
	}

	void TimerQueue::siftDown(int n)
	{
		// While we have children bigger than us, swap us with the biggest child.
		while (lchild(n) < _numEntries)
		{
			int bigc = lchild(n);
			if (rchild(n) < _numEntries && pri(rchild(n)) > pri(bigc))
			{
				bigc = rchild(n);
			}
			if (pri(bigc) <= pri(n))
			{
				break;
			}
			swap(n, bigc);
			n = bigc;
		}
	}

	void TimerQueue::siftUp(int n)
	{
		while ((n != 0) && (_table[n].pri > _table[parent(n)].pri))
		{
			swap(n, parent(n));
			n = parent(n);
		}
		siftDown(n);
	}

	void TimerQueue::growArray()
	{
		_tableSize = ((_tableSize + 1) << 1) - 1;
		HeapEntry *newTable = new HeapEntry[_tableSize];
		for (int i = 0; i < _numEntries; i++)
		{
			newTable[i].pri = _table[i].pri;
			newTable[i].timer = _table[i].timer;
		}
		delete[] _table;
		_table = newTable;
	}

	Timer* TimerQueue::findByName(const std::string& name) const
	{
		for (int i = 0; i < _numEntries; ++i)
		{
			if (_table[i].timer->name == name)
			{
				return _table[i].timer;
			}
		}
		return NULL;
	}
}
