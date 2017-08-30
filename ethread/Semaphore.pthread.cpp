/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <etk/types.h>
#include <ethread/Semaphore.h>
#include <ethread/debug.h>
#include <sys/time.h>

ethread::Semaphore::Semaphore(uint32_t _nbBasicElement, uint32_t _nbMessageMax) {
	// create interface mutex :
	int ret = pthread_mutex_init(&m_mutex, nullptr);
	TK_ASSERT(ret == 0, "Error creating Mutex ...");
	// create contition :
	ret = pthread_cond_init(&m_condition, nullptr);
	TK_ASSERT(ret == 0, "Error creating Condition ...");
	if (ret != 0) {
		ret = pthread_mutex_destroy(&m_mutex);
		TK_ASSERT(ret == 0, "Error destroying Mutex ...");
	}
	m_maximum = _nbMessageMax;
	m_data = _nbBasicElement;
}


ethread::Semaphore::~Semaphore() {
	// Remove condition
	int ret = pthread_cond_destroy(&m_condition);
	TK_ASSERT(ret == 0, "Error destroying Condition ...");
	// Remove Mutex
	ret = pthread_mutex_destroy(&m_mutex);
	TK_ASSERT(ret == 0, "Error destroying Mutex ...");
}

uint32_t ethread::Semaphore::getCount() {
	int32_t tmpData = 0;
	pthread_mutex_lock(&m_mutex);
	tmpData = m_data;
	pthread_mutex_unlock(&m_mutex);
	return tmpData;
}

void ethread::Semaphore::post() {
	pthread_mutex_lock(&m_mutex);
	if (m_data>=m_maximum) {
		m_data = m_maximum;
	} else {
		m_data++;
	}
	// send message
	pthread_cond_broadcast(&m_condition);
	pthread_mutex_unlock(&m_mutex);
}


void ethread::Semaphore::wait() {
	pthread_mutex_lock(&m_mutex);
	while(m_data == 0) {
		pthread_cond_wait(&m_condition, &m_mutex);
	}
	m_data--;
	pthread_mutex_unlock(&m_mutex);
}


bool ethread::Semaphore::wait(uint64_t _timeOutInUs) {
	pthread_mutex_lock(&m_mutex);
	if(m_data == 0) {
		struct timeval tp;
		struct timespec ts;
		gettimeofday(&tp, nullptr);
		uint64_t totalTimeUS = tp.tv_sec * 1000000 + tp.tv_usec;
		totalTimeUS += _timeOutInUs;
		ts.tv_sec = totalTimeUS / 1000000;
		ts.tv_nsec = (totalTimeUS%1000000) * 1000;
		int ret = pthread_cond_timedwait(&m_condition, &m_mutex, &ts);
		if (ret !=0) { //== ETIMEOUT) {
			pthread_mutex_unlock(&m_mutex);
			return false;
		}
	}
	m_data--;
	pthread_mutex_unlock(&m_mutex);
	return true;
}

