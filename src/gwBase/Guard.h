#ifndef GWBASE_GUARD_H
#define GWBASE_GUARD_H 1

#include <compiler.h>

/**
 * A scoped locking utility.
 * An GWGuard object locks its synchronization object during creation and
 * automatically unlocks it when it goes out of scope.
 */
template<class SGLOCK>
class GWGuard
{
public:

	/**
	 * Create an GWGuard object and lock the passed lockable object.
	 * @param l A lockable object.
	 */
	inline GWGuard(SGLOCK& l) : lock(l) { lock.lock(); }

	/**
	 * Destroy this object and unlock the lockable object.
	 */
	inline ~GWGuard() { lock.unlock(); }

private:

	SGLOCK& lock; //!< A lockable object

private:
	// Disable copying.
	GWGuard(const SGLOCK&);
	SGLOCK& operator= (const SGLOCK&);
};

#endif // GWGUARD_H_INCLUDED
