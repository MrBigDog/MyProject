//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_safe_ptr.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : radian template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_SAFE_PTR_H_
#define _US_SAFE_PTR_H_

namespace uniscope_globe
{
	template<class T>
	class secure_ptr
	{
	protected:
		T* pointer_value;

		int* use_count;

	public:
		US_AUTO_SHARED_MUTEX

			secure_ptr()
			: pointer_value(0)
			, use_count(0)
		{
			US_SET_AUTO_SHARED_MUTEX_NULL
		}

		explicit secure_ptr(T* rep)
			: pointer_value(rep)
			, use_count(new int(0))
		{
			US_SET_AUTO_SHARED_MUTEX_NULL
				US_NEW_AUTO_SHARED_MUTEX
		}
		secure_ptr(const secure_ptr& r)
			: pointer_value(0)
			, use_count(0)
		{
			// lock & copy other mutex pointer
			US_SET_AUTO_SHARED_MUTEX_NULL
				US_MUTEX_CONDITIONAL(r.US_AUTO_MUTEX_NAME)
			{
				US_LOCK_MUTEX(*r.US_AUTO_MUTEX_NAME)
					US_COPY_AUTO_SHARED_MUTEX(r.US_AUTO_MUTEX_NAME)

					pointer_value = r.pointer_value;
				use_count = r.use_count;

				// Handle zero pointer gracefully to manage STL containers
				if (use_count)
				{
					++(*use_count);
				}
			}
		}

		secure_ptr& operator = (const secure_ptr& r)
		{
			if (pointer_value == r.pointer_value)
			{
				return *this;
			}

			release();

			// lock & copy other mutex pointer
			US_MUTEX_CONDITIONAL(r.US_AUTO_MUTEX_NAME)
			{
				US_LOCK_MUTEX(*r.US_AUTO_MUTEX_NAME)
					US_COPY_AUTO_SHARED_MUTEX(r.US_AUTO_MUTEX_NAME)

					pointer_value = r.pointer_value;
				use_count = r.use_count;
				if (use_count)
				{
					++(*use_count);
				}
			}

			return *this;
		}

		virtual ~secure_ptr()
		{
			release();
		}

		inline T& operator*() const
		{
			assert(pointer_value);
			return *pointer_value;
		}

		inline T* operator->() const
		{
			assert(pointer_value);
			return pointer_value;
		}

		inline T* get_ptr() const
		{
			return pointer_value;
		}

		// Binds rep to the secure_ptr @remarks Assumes that the secure_ptr is uninitialised!
		void bind(T* rep)
		{
			assert(!pointer_value && !use_count);

			US_NEW_AUTO_SHARED_MUTEX
				US_LOCK_AUTO_SHARED_MUTEX

				use_count = new unsigned int(1);
			pointer_value = rep;
		}

		inline bool unique() const
		{
			US_LOCK_AUTO_SHARED_MUTEX
				assert(use_count);
			return *use_count == 1;
		}

		inline unsigned int useCount() const
		{
			US_LOCK_AUTO_SHARED_MUTEX
				assert(use_count);
			return *use_count;
		}

		inline unsigned int* use_count_pointer() const
		{
			return use_count;
		}

		inline T* get_pointer() const
		{
			return pointer_value;
		}

		inline bool is_null(void) const
		{
			return pointer_value == 0;
		}

		inline void set_null(void)
		{
			if (pointer_value)
			{
				// can't scope lock mutex before release incase deleted
				release();
				pointer_value = 0;
				use_count = 0;
			}
		}

	protected:

		inline void release(void)
		{
			bool destroyThis = false;

			// If the mutex is not initialized to a non-zero value, then neither is use_count nor pointer_value.
			US_MUTEX_CONDITIONAL(US_AUTO_MUTEX_NAME)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				US_LOCK_AUTO_SHARED_MUTEX

					if (use_count)
					{
						if (--(*use_count) == 0)
						{
							destroyThis = true;
						}
					}
			}
			if (destroyThis)
			{
				destroy();
			}

			US_SET_AUTO_SHARED_MUTEX_NULL
		}

		virtual void destroy(void)
		{
			delete pointer_value;
			delete use_count;
			US_DEL_AUTO_SHARED_MUTEX
		}
	};

	template<class T, class U> inline bool operator == (secure_ptr<T> const& a, secure_ptr<U> const& b)
	{
		return a.get_ptr() == b.get_ptr();
	}

	template<class T, class U> inline bool operator != (secure_ptr<T> const& a, secure_ptr<U> const& b)
	{
		return a.get_ptr() != b.get_ptr();
	}

} // namespace uniscope_globe

#endif // #ifndef _US_SAFE_PTR_H_