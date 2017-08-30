/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.h>
#include <ethread/Mutex.h>
#include <ethread/debug.h>

ethread::Mutex::Mutex() {
	// create interface mutex :
	int ret = pthread_mutex_init(&m_mutex, nullptr);
	ETHREAD_ASSERT(ret == 0, "Error creating Mutex ...");
}


ethread::Mutex::~Mutex() {
	// Remove mutex
	int ret = pthread_mutex_destroy(&m_mutex);
	ETHREAD_ASSERT(ret == 0, "Error destroying Mutex ...");
}


void ethread::Mutex::lock() {
	pthread_mutex_lock(&m_mutex);
}


bool ethread::Mutex::tryLock() {
	return pthread_mutex_trylock(&m_mutex) != 0;
}


void ethread::Mutex::unLock() {
	pthread_mutex_unlock(&m_mutex);
}

