/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ethread/Pool.hpp>
#include <ethread/PoolExecutor.hpp>
#include "debug.hpp"
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::Pool);

ethread::Pool::Pool(uint16_t _numberOfThread):
  m_lastTrandId(1) {
	ethread::UniqueLock lock(m_mutex);
	for (uint32_t iii=0; iii<_numberOfThread; ++iii) {
		ememory::SharedPtr<ethread::PoolExecutor> tmp = ememory::makeShared<ethread::PoolExecutor>(*this);
		if (tmp != null) {
			tmp->start();
			m_listThread.pushBack(tmp);
		}
	}
}

ethread::Pool::~Pool() {
	stop();
	join();
}

uint32_t ethread::Pool::createGroupId() {
	ethread::UniqueLock lock(m_mutex);
	return m_lastTrandId++;
}

ethread::Future ethread::Pool::async(etk::Function<void()> _call, uint64_t _executionInGroupId) {
	ethread::UniqueLock lock(m_mutex);
	if (_call == null) {
		ETHREAD_ERROR("Can not add an action with no function to call...");
		return ethread::Future();
	}
	ememory::SharedPtr<ethread::Promise> promise = ememory::makeShared<ethread::Promise>();
	ememory::SharedPtr<ethread::PoolAction> action = ememory::makeShared<ethread::PoolAction>(_executionInGroupId, promise, _call);
	m_listActions.pushBack(action);
	for(auto &it : m_listThread) {
		if (it == null) {
			continue;
		}
		if (it->touch() == true) {
			// Find one to force action now ...
			break;
		}
	}
	return ethread::Future(promise);
}

void ethread::Pool::releaseId(uint64_t _id) {
	if (_id == 0) {
		return;
	}
	ethread::UniqueLock lock(m_mutex);
	auto it = m_listIdPool.begin();
	while (it != m_listIdPool.end()) {
		if (*it == _id) {
			it = m_listIdPool.erase(it);
			continue;
		}
		++it;
	}
}

// get an action to execute ...
ememory::SharedPtr<ethread::PoolAction> ethread::Pool::getAction() {
	ethread::UniqueLock lock(m_mutex);
	auto it = m_listActions.begin();
	while (it != m_listActions.end()) {
		if (*it == null) {
			it = m_listActions.erase(it);
			continue;
		}
		// Check if this element is associated at a specific pool...
		uint64_t uniquId = (*it)->getPoolId();
		bool alreadyUsed = false;
		if (uniquId != 0) {
			for (auto &itId : m_listIdPool) {
				if (itId == uniquId) {
					alreadyUsed = true;
					break;
				}
			}
		}
		if (alreadyUsed == false) {
			ememory::SharedPtr<ethread::PoolAction> out = (*it);
			if (uniquId != 0) {
				m_listIdPool.pushBack(uniquId);
			}
			it = m_listActions.erase(it);
			return out;
		}
		++it;
	}
	return null;
}


void ethread::Pool::stop() {
	ethread::UniqueLock lock(m_mutex);
	auto it = m_listThread.begin();
	while (it != m_listThread.end()) {
		if (*it == null) {
			it = m_listThread.erase(it);
			continue;
		}
		(*it)->stop();
		++it;
	}
}

void ethread::Pool::join() {
	ethread::UniqueLock lock(m_mutex);
	ETHREAD_DEBUG("start join all the threads in pool " << m_listThread.size());
	for (size_t iii=0; iii<m_listThread.size(); ++iii) {
		ETHREAD_DEBUG("    join " << iii);
		if (m_listThread[iii] == null) {
			continue;
		}
		m_listThread[iii]->join();
	}
	ETHREAD_DEBUG("    ==> all joined");
	m_listThread.clear();
	ETHREAD_DEBUG("    ==> all reset");
}

