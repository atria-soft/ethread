/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <mutex>
#include <vector>
#include <thread>
#include <ethread/Future.hpp>
#include <ethread/PoolExecutor.hpp>
#include <ethread/tools.hpp>
#include "debug.hpp"

ethread::PoolExecutor::PoolExecutor(ethread::Pool& _pool):
  m_pool(_pool),
  m_running(false),
  m_uniqueId(0) {
	static uint32_t uid = 10;
	m_uniqueId = uid++;
}

void ethread::PoolExecutor::threadCallback() {
	ETHREAD_INFO("[" << m_uniqueId << "] RUN: thread in Pool [START]");
	ethread::setName("pool " + etk::to_string(m_uniqueId));
	// get datas:
	while (m_running == true) {
		// get an action:
		m_action = m_pool.getAction();
		if (m_action == nullptr) {
			// TODO : This is really bad ==> fast to code and debug but not optimum at all ... use condition instead ...
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		m_action->call();
		m_pool.releaseId(m_action->getPoolId());
		m_action.reset();
	}
	m_running = false;
	ETHREAD_INFO("[" << m_uniqueId << "] RUN: thread in Pool [STOP]");
}

void ethread::PoolExecutor::start() {
	ETHREAD_INFO("START: thread in Pool [START]");
	m_running = true;
	m_thread = ememory::makeShared<std::thread>([&](void *){ this->threadCallback();}, nullptr);
	if (m_thread == nullptr) {
		m_running = false;
		ETHREAD_ERROR("START: thread in Pool [STOP] can not intanciate THREAD!");
		return;
	}
	//ethread::setPriority(*m_receiveThread, -6);
	ETHREAD_INFO("START: thread in Pool [STOP]");
}

void ethread::PoolExecutor::stop() {
	ETHREAD_INFO("[" << m_uniqueId << "] STOP: thread in Pool [START]");
	m_running = false;
	ETHREAD_INFO("[" << m_uniqueId << "] STOP: thread in Pool [STOP]");
}

void ethread::PoolExecutor::join() {
	ETHREAD_INFO("[" << m_uniqueId << "] JOIN: thread in Pool [START]");
	if (m_thread != nullptr) {
		ETHREAD_INFO("[" << m_uniqueId << "] JOIN: waiting ...");
		m_thread->join();
		m_thread.reset();
	}
	ETHREAD_INFO("[" << m_uniqueId << "] JOIN: thread in Pool [STOP]");
}



