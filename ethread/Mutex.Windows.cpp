/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ethread/Mutex.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::Mutex);
ETK_DECLARE_TYPE(ethread::UniqueLock);

ethread::Mutex::Mutex() {
	InitializeCriticalSection(&m_mutex);
}

ethread::Mutex::~Mutex() {
	DeleteCriticalSection(&m_mutex);
}

void ethread::Mutex::lock() {
	EnterCriticalSection(&m_mutex);
}

bool ethread::Mutex::tryLock() {
	return TryEnterCriticalSection(&m_mutex) != 0;
}

void ethread::Mutex::unLock() {
	LeaveCriticalSection(&m_mutex);
}

