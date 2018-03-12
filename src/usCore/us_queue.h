#ifndef MDQueue_h__
#define MDQueue_h__
////add by Felix
#include <usCore\Export.h>
#include <cassert>
#include <queue>

namespace uniscope_globe
{

	/**
	* MDQueue defines an interface for a FIFO.
	* It can be implemented using different types of synchronization
	* and protection.
	*/
	template<class T>
	class GWQueue
	{
	public:
		/*** Create a new MDQueue object.*/
		GWQueue() {}


		/** * Destroy this object.*/
		virtual ~GWQueue() {}

		/**
		* Returns whether this queue is empty (contains no elements).
		*
		* @return bool True if queue is empty, otherwisr false.
		*/
		virtual bool empty() = 0;

		/**
		* Add an item to the end of the queue.
		*
		* @param item  object to add.
		*/
		virtual void push(const T& item) = 0;

		/**
		* View the item from the head of the queue.
		*
		* @return The next available object.
		*/
		virtual T front() = 0;

		/**
		* Get an item from the head of the queue.
		*
		* @return The next available object.
		*/
		virtual T pop() = 0;

		/**
		* Query the size of the queue
		*
		* @return size_t size of queue.
		*/
		virtual size_t size() = 0;

	protected:
		std::queue<T> fifo;
	};

	/**
	* A simple thread safe queue.  All access functions are guarded with a mutex.
	*/
	template<class T>
	class GWLockedQueue : public GWQueue<T>
	{
	public:

		/**
		* Create a new MDLockedQueue object.
		*/
		GWLockedQueue() {}

		/**
		* Destroy this object.
		*/
		virtual ~GWLockedQueue() {}

		/**
		* Returns whether this queue is empty (contains no elements).
		*
		* @return True if queue is empty, otherwise false.
		*/
		virtual bool empty()
		{
			US_LOCK_AUTO_MUTEX
				return this->fifo.empty();
		}

		/**
		* Add an item to the end of the queue.
		*
		* @param item object to add.
		*/
		virtual void push(const T& item) {
			US_LOCK_AUTO_MUTEX
				this->fifo.push(item);
		}

		/**
		* View the item from the head of the queue.
		*
		* @return The next available object.
		*/
		virtual T front() {
			US_LOCK_AUTO_MUTEX
				assert(!this->fifo.empty());
			T item = this->fifo.front();
			return item;
		}

		/**
		* Get an item from the head of the queue.
		*
		* @return The next available object.
		*/
		virtual T pop() {
			US_LOCK_AUTO_MUTEX
				if (this->fifo.empty()) return T(); // assumes T is default constructable

			//  	if (fifo.empty())
			//  	{
			//  	    mutex.unlock();
			//  	    pthread_exit( PTHREAD_CANCELED );
			//  	}
			T item = this->fifo.front();
			this->fifo.pop();
			return item;
		}

		/**
		* Query the size of the queue
		*
		* @return Size of queue.
		*/
		virtual size_t size() {
			US_LOCK_AUTO_MUTEX
				return this->fifo.size();
		}

	private:

		/**
		* Mutex to serialise access.
		*/
		US_AUTO_MUTEX

	private:
		// Prevent copying.
		GWLockedQueue(const GWLockedQueue&);
		GWLockedQueue& operator= (const GWLockedQueue&);
	};

	/**
	* A guarded queue blocks threads trying to retrieve items
	* when none are available.
	*/
	template<class T>
	class GWBlockingQueue : public GWQueue<T>
	{
	public:
		GWBlockingQueue() {}
		virtual ~GWBlockingQueue() {}

		virtual bool empty()
		{
			US_LOCK_AUTO_MUTEX
				return this->fifo.empty();
		}

		/**
		* Add an item to the end of the queue.
		*
		* @param item The object to add.
		*/
		virtual void push(const T& item)
		{
			US_LOCK_AUTO_MUTEX
				this->fifo.push(item);
			not_empty.signal();
		}

		/**
		* View the item from the head of the queue.
		* Calling thread is not suspended
		*
		* @return The next available object.
		*/
		virtual T front()
		{
			US_LOCK_AUTO_MUTEX

				assert(this->fifo.empty() != true);
			//if (fifo.empty()) throw ??

			T item = this->fifo.front();
			return item;
		}

		/**
		* Get an item from the head of the queue.
		* If no items are available then the calling thread is suspended
		*
		* @return The next available object.
		*/
		virtual T pop()
		{
			US_LOCK_AUTO_MUTEX

				while (this->fifo.empty())
				{
					not_empty.wait(mutex);
				}


				assert(this->fifo.empty() != true);
				//if (fifo.empty()) throw ??

				T item = this->fifo.front();
				this->fifo.pop();
				return item;
		}

		/**
		* Query the size of the queue
		*
		* @return Size of queue.
		*/
		virtual size_t size()
		{
			US_LOCK_AUTO_MUTEX
				return this->fifo.size();
		}

	private:

		/**
		* Mutex to serialise access.
		*/
		US_AUTO_MUTEX;

		/**
		* Condition to signal when queue not empty.
		*/
		GWWaitCondition not_empty;

	private:
		// Prevent copying.
		GWBlockingQueue(const GWBlockingQueue&);
		GWBlockingQueue& operator=(const GWBlockingQueue&);
	};


	/**
	* A guarded deque blocks threads trying to retrieve items
	* when none are available.
	*/
	template<class T>
	class GWBlockingDeque
	{
	public:

		/*** Create a new GWBlockingDequeue.*/
		GWBlockingDeque() {}

		/*** Destroy this dequeue.*/
		virtual ~GWBlockingDeque() {}

		virtual void clear()
		{
			US_LOCK_AUTO_MUTEX
				this->queue.clear();
		}

		virtual bool empty()
		{
			US_LOCK_AUTO_MUTEX
				return this->queue.empty();
		}

		/**
		* Add an item to the front of the queue.
		*
		* @param item The object to add.
		*/
		virtual void push_front(const T& item)
		{
			US_LOCK_AUTO_MUTEX
				this->queue.push_front(item);
			not_empty.signal();
		}

		/**
		* Add an item to the back of the queue.
		*
		* @param item The object to add.
		*/
		virtual void push_back(const T& item)
		{
			US_LOCK_AUTO_MUTEX
				this->queue.push_back(item);
			not_empty.signal();
		}

		/**
		* View the item from the head of the queue.
		* Calling thread is not suspended
		*
		* @return The next available object.
		*/
		virtual T front()
		{
			US_LOCK_AUTO_MUTEX

				assert(this->queue.empty() != true);
			//if (queue.empty()) throw ??

			T item = this->queue.front();
			return item;
		}

		/**
		* Get an item from the head of the queue.
		* If no items are available then the calling thread is suspended
		*
		* @return The next available object.
		*/
		virtual T pop_front()
		{
			US_LOCK_AUTO_MUTEX

				while (this->queue.empty())
				{
					not_empty.wait(mutex);
				}

				assert(this->queue.empty() != true);
				//if (queue.empty()) throw ??

				T item = this->queue.front();
				this->queue.pop_front();
				return item;
		}

		/**
		* Get an item from the tail of the queue.
		* If no items are available then the calling thread is suspended
		*
		* @return The next available object.
		*/
		virtual T pop_back()
		{
			US_LOCK_AUTO_MUTEX

				while (this->queue.empty())
				{
					not_empty.wait(mutex);
				}

				assert(this->queue.empty() != true);
				//if (queue.empty()) throw ??

				T item = this->queue.back();
				this->queue.pop_back();
				return item;
		}

		/**
		* Query the size of the queue
		*
		* @return Size of queue.
		*/
		virtual size_t size()
		{
			US_LOCK_AUTO_MUTEX
				return this->queue.size();
		}

		void waitOnNotEmpty()
		{
			US_LOCK_AUTO_MUTEX
				while (this->queue.empty())
				{
					not_empty.wait(mutex);
				}
		}

	private:
		/*** Mutex to serialise access.*/
		US_AUTO_MUTEX


			/*** Condition to signal when queue not empty.*/
			GWWaitCondition not_empty;

	private:
		// Prevent copying.
		GWBlockingDeque(const GWBlockingDeque&);
		GWBlockingDeque& operator=(const GWBlockingDeque&);

	protected:
		std::deque<T> queue;
	};

}

#endif // MDQueue_h__
