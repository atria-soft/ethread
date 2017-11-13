/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ethread/MutexRecursive.hpp>
#include <ethread/tools.hpp>
extern "C" {
	#include <errno.h>
}
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::MutexRecursive);
ETK_DECLARE_TYPE(ethread::RecursiveLock);
//#include <ethread/debug.hpp>

ethread::MutexRecursive::MutexRecursive() {
	#ifdef DEBUG
		m_threadThatHaveLock = 0xFFFFFFFF;
	#endif
	// create interface mutex :
	pthread_mutexattr_init(&m_attribute);
	pthread_mutexattr_settype(&m_attribute, PTHREAD_MUTEX_RECURSIVE);
	int ret = pthread_mutex_init(&m_mutex, &m_attribute);
	//ETHREAD_ASSERT(ret == 0, "Error creating Mutex ...");
}


ethread::MutexRecursive::~MutexRecursive() {
	// Remove mutex
	int ret = pthread_mutex_destroy(&m_mutex);
	//ETHREAD_ASSERT(ret == 0, "Error destroying Mutex ...");
}


void ethread::MutexRecursive::lock() {
	pthread_mutex_lock(&m_mutex);
	#ifdef DEBUG
		m_threadThatHaveLock = ethread::getId();
	#endif
}


bool ethread::MutexRecursive::tryLock() {
	int ret = pthread_mutex_trylock(&m_mutex);
	if (ret == 0) {
		return true;
	}
	if (ret == EINVAL) {
		printf("trylock error: EINVAL\n");
		// The mutex was created with the protocol attribute having the value PTHREAD_PRIO_PROTECT and the calling thread's
		// priority is higher than the mutex's current priority ceiling.
		// The pthread_mutex_trylock() function shall fail if:
	}
	if (ret == EBUSY) {
		printf("trylock error: EBUSY\n");
		// The mutex could not be acquired because it was already locked.
		// The pthread_mutex_lock(), pthread_mutex_trylock(), and pthread_mutex_unlock() functions may fail if:
	}
	if (ret == EINVAL) {
		printf("trylock error: EINVAL\n");
		// The value specified by mutex does not refer to an initialized mutex object.
	}
	if (ret == EAGAIN) {
		printf("trylock error: EAGAIN\n");
		// The mutex could not be acquired because the maximum number of recursive locks for mutex has been exceeded.
		// The pthread_mutex_lock() function may fail if:
	}
	if (ret == EDEADLK) {
		printf("trylock error: EDEADLK\n");
		// The current thread already owns the mutex.
		// The pthread_mutex_unlock() function may fail if:
	}
	if (ret == EPERM) {
		printf("trylock error: EPERM\n");
		//The current thread does not own the mutex.
	}
	return false;
}


void ethread::MutexRecursive::unLock() {
	pthread_mutex_unlock(&m_mutex);
	#ifdef DEBUG
		m_threadThatHaveLock = 0xFFFFFFFF;
	#endif
}

