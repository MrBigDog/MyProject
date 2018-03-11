//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_heap.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : io stream
//	Reference : http://www.krugle.org/examples/p-bgQIhj5zFpYC0A6L/MxBlock.h
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_HEAP_H_
#define _US_HEAP_H_

namespace uniscope_globe
{
	template<class T>
	class heap_block
	{
	private:
		uint num;
		T *block;

	protected:
		heap_block() { }

		void init_block(uint n)
		{
			// Allocate memory for block
			num = n;
			block = (T*)malloc(sizeof(T) * n);

			// Initialize memory
			memset(block, 0, sizeof(T) * n);
		}

		void resize_block(uint n)
		{
			T* old = block;

			// Allocate new block
			block = (T*)realloc(old, sizeof(T) * n);

			// Initialize memory
			for (uint i = num; i < n; i++)
			{
				new((void *)&block[i]) T;
			}

			num = n;
		}

		void free_block()
		{
			for (uint i = 0; i < num; i++)
			{
				T *p = &block[i];
				p->~T();
			}

			free(block);
		}

		T&       raw(uint i) { return block[i]; }

		const T& raw(uint i) const { return block[i]; }

	public:
		heap_block(uint n) { init_block(n); }

		virtual ~heap_block() { free_block(); }

		bool operator == (const heap_block& x) const { return this == &x; }

		// Standard accessors
		T&       ref(uint i) { assert(i < num); return block[i]; }

		const T& ref(uint i) const { assert(i < num); return block[i]; }

		T&       operator() (uint i) { return ref(i); }

		const T& operator()(uint i) const { return ref(i); }

		operator const T*() const { return block; }

		operator T*() { return block; }

		uint length() const { return num; }

		// Primitive methods for altering the data block
		void resize(uint n) { resize_block(n); }

		// copy bits directly
		void bitcopy(const T *array, uint n) { memcpy(block, array, MIN(n, num) * sizeof(T)); }

		// copy using assignment operator
		void copy(const T *array, const uint n) { for (uint i = 0; i < MIN(n, num); i++) block[i] = array[i]; }

		void bitcopy(const heap_block<T>& b) { bitcopy(b, b.length()); }

		void copy(const heap_block<T>& b) { copy(b, b.length()); }

	};

	template<class T>
	class dynamic_block : public heap_block<T>
	{
	private:
		uint fill;

	public:
		dynamic_block() { init_block(8); fill = 0; }
		dynamic_block(uint n) { init_block(n); fill = 0; }

		T&       operator()(uint i) { assert(i < fill); return ref(i); }
		const T& operator()(uint i) const { assert(i < fill); return ref(i); }

		uint length() const { return fill; }
		uint total_space() const { return heap_block<T>::length(); }

		T& last() { assert(fill > 0); return raw(fill - 1); }
		const T& last() const { assert(fill > 0); return raw(fill - 1); }

		uint add()
		{
			if (length() == total_space())  resize(total_space() * 2);
			return fill++;
		}

		uint add(const T& t)
		{
			uint i = add();
			raw(i) = t;
			return i;
		}

		void reset() { fill = 0; }

		T& drop() { return raw(--fill); }

		void drop(uint d) { fill -= d; }
		bool find(const T& t, uint *index = NULL)
		{
			for (uint i = 0; i < fill; i++)
			{
				if (raw(i) == t)
				{
					if (index) *index = i;
					return true;
				}
			}

			return false;
		}

		void remove(uint i) { assert(i < fill); fill--; raw(i) = raw(fill); }
		void remove_inorder(uint i) { memmove(&raw(i), &raw(i + 1), (--fill - i) * sizeof(T)); }
	};

	template<class T, int t_size>
	class sized_dynamic_block : public dynamic_block<T>
	{
	public:
		sized_dynamic_block(uint n = t_size)
			: dynamic_block<T>(n)
		{ }
	};


	template<class T>
	class heapable
	{
	private:
		T import;
		int token;

	public:
		heapable() { not_in_heap(); heap_key(0); }

		inline bool		is_in_heap() { return token != -47; }
		inline void		not_in_heap() { token = -47; }
		inline int		get_heap_pos() { return token; }
		inline void		set_heap_pos(int t) { token = t; }

		inline void		heap_key(T k) { import = k; }
		inline T		heap_key() const { return import; }
	};

	template<class T>
	class heap_base : public dynamic_block<heapable<T> *>
	{
	private:
		void place(heapable<T> *x, uint i);
		void swap(uint i, uint j);

		uint parent(uint i) { return (i - 1) / 2; }
		uint left(uint i) { return 2 * i + 1; }
		uint right(uint i) { return 2 * i + 2; }

		void upheap(uint i);
		void downheap(uint i);

	public:
		// construct
		heap_base()
			: dynamic_block<heapable<T> *>(8)
		{ }

		heap_base(uint n)
			: dynamic_block<heapable<T> *>(n)
		{ }

		void insert(heapable<T> *t) { insert(t, t->heap_key()); }
		void insert(heapable<T> *, T);
		void update(heapable<T> *t) { update(t, t->heap_key()); }
		void update(heapable<T> *, T);

		uint size() const { return length(); }
		heapable<T>* item(uint i) { return ref(i); }
		const heapable<T>* item(uint i) const { return ref(i); }
		heapable<T>* extract();
		heapable<T>* top() { return (length() < 1 ? (heapable<T> *)NULL : raw(0)); }
		heapable<T>* remove(heapable<T> *);
	};

	template<class T>
	inline void heap_base<T>::place(heapable<T> *x, uint i)
	{
		ref(i) = x;
		x->set_heap_pos(i);
	}

	template<class T>
	void heap_base<T>::swap(uint i, uint j)
	{
		heapable<T> *tmp = ref(i);

		place(ref(j), i);
		place(tmp, j);
	}

	template<class T>
	void heap_base<T>::upheap(uint i)
	{
		heapable<T> *moving = ref(i);
		uint index = i;
		uint p = parent(i);

		while (index > 0 && moving->heap_key() > ref(p)->heap_key())
		{
			place(ref(p), index);
			index = p;
			p = parent(p);
		}

		if (index != i)
		{
			place(moving, index);
		}
	}

	template<class T>
	void heap_base<T>::downheap(uint i)
	{
		heapable<T> *moving = ref(i);
		uint index = i;
		uint l = left(i);
		uint r = right(i);
		uint largest;

		while (l < length())
		{
			if (r < length() && ref(l)->heap_key() < ref(r)->heap_key())
			{
				largest = r;
			}
			else
			{
				largest = l;
			}

			if (moving->heap_key() < ref(largest)->heap_key())
			{
				place(ref(largest), index);
				index = largest;
				l = left(index);
				r = right(index);
			}
			else
			{
				break;
			}
		}

		if (index != i)
		{
			place(moving, index);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//
	// Exported interface to the heap
	//

	template<class T>
	void heap_base<T>::insert(heapable<T>* t, T v)
	{
		t->heap_key(v);

		uint i = add(t);
		t->set_heap_pos(i);

		upheap(i);
	}

	template<class T>
	void heap_base<T>::update(heapable<T>* t, T v)
	{
		//   assert( t->is_in_heap() );
		t->heap_key(v);

		int i = t->get_heap_pos();
		if (i == -47)
			return;

		if (i > 0 && v > ref(parent(uint(i)))->heap_key())
		{
			upheap((uint)i);
		}
		else
		{
			downheap((uint)i);
		}
	}

	template<class T>
	heapable<T> *heap_base<T>::extract()
	{
		if (length() < 1) return NULL;

		swap(0, length() - 1);
		heapable<T> *dead = drop();

		downheap(0);
		dead->not_in_heap();
		return dead;
	}

	template<class T>
	heapable<T> *heap_base<T>::remove(heapable<T> *t)
	{
		if (!t->is_in_heap()) return NULL;

		int i = t->get_heap_pos();
		swap(i, length() - 1);
		drop();
		t->not_in_heap();

		if (ref(i)->heap_key() < t->heap_key())
		{
			downheap(i);
		}
		else
		{
			upheap(i);
		}

		return t;
	}

} // uniscope_globe

#endif // _US_IO_STREAM_H_
