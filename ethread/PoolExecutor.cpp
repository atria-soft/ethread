/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ethread/Mutex.hpp>
#include <etk/Vector.hpp>
#include <ethread/Thread.hpp>
#include <ethread/Future.hpp>
#include <ethread/PoolExecutor.hpp>
#include <ethread/tools.hpp>
#include "debug.hpp"
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::PoolExecutor);

ethread::PoolExecutor::PoolExecutor(ethread::Pool& _pool):
  m_needProcess(false),
  m_isWaiting(false),
  m_pool(_pool),
  m_running(false) {
	
}

void ethread::PoolExecutor::threadCallback() {
	ETHREAD_DEBUG("RUN: thread in Pool [START]");
	ethread::setName("pool " + etk::toString(ethread::getId()));
	// get datas:
	while (m_running == true) {
		// get an action:
		m_action = m_pool.getAction();
		if (m_action == nullptr) {
			// If no action availlable and not requested to check, just sleep ...
			if (m_needProcess == false) {
				m_isWaiting = true;
				ETHREAD_VERBOSE("RUN: Jump in sleep");
				if (m_semaphore.wait(60000000) == false) {
					ETHREAD_VERBOSE("RUN: time-out");
					continue;
				}
				ETHREAD_VERBOSE("RUN: WakeUp");
				m_isWaiting = false;
				m_needProcess = false;
			}
			continue;
		}
		m_action->call();
		m_pool.releaseId(m_action->getPoolId());
		m_action.reset();
	}
	m_running = false;
	ETHREAD_DEBUG("RUN: thread in Pool [STOP]");
}

void ethread::PoolExecutor::start() {
	ETHREAD_DEBUG("START: thread in Pool [START]");
	m_running = true;
	m_semaphore.post();
	m_thread = ememory::makeShared<ethread::Thread>([&](){ threadCallback();});
	if (m_thread == nullptr) {
		m_running = false;
		ETHREAD_ERROR("START: thread in Pool [STOP] can not intanciate THREAD!");
		return;
	}
	//ethread::setPriority(*m_receiveThread, -6);
	ETHREAD_DEBUG("START: thread in Pool [STOP]");
}

void ethread::PoolExecutor::stop() {
	ETHREAD_DEBUG("STOP: thread in Pool [START]");
	m_semaphore.post();
	m_running = false;
	ETHREAD_DEBUG("STOP: thread in Pool [STOP]");
}

void ethread::PoolExecutor::join() {
	ETHREAD_DEBUG("JOIN: thread in Pool [START]");
	m_semaphore.post();
	if (m_thread != nullptr) {
		ETHREAD_DEBUG("JOIN: waiting ...");
		m_thread->join();
		m_thread.reset();
	}
	ETHREAD_DEBUG("JOIN: thread in Pool [STOP]");
}

bool ethread::PoolExecutor::touch() {
	bool ret = false;
	if (    m_needProcess == false
	     && m_isWaiting == true) {
		ETHREAD_VERBOSE("Touch ...");
		m_needProcess = true;
		ret = true;
		m_semaphore.post();
	}
	return ret;
}





