/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include "debug.hpp"
#include <ethread/Future.hpp>
#include <ethread/Promise.hpp>
#include <ethread/tools.hpp>
#include <echrono/Steady.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::Promise);

ethread::Promise::Promise():
  m_isFinished(false) {
	
}

bool ethread::Promise::isFinished() {
	return m_isFinished;
}

void ethread::Promise::finish() {
	etk::Function<void()> callback;
	{
		ethread::UniqueLock lock(m_mutex);
		if (m_isFinished == true) {
			ETHREAD_ERROR("Request 2 time finishing a Promise ...");
			return;
		}
		m_isFinished = true;
		if (m_callback != null) {
			// call callbacks ...
			callback = etk::move(m_callback);
		}
	}
	if (callback != null) {
		callback();
	}
}

bool ethread::Promise::wait(echrono::Duration _delay) {
	echrono::Steady time = echrono::Steady::now();
	while (_delay >= 0) {
		{
			ethread::UniqueLock lock(m_mutex);
			if (m_isFinished == true) {
				return true;
			}
		}
		echrono::Steady time2 = echrono::Steady::now();
		_delay -= (time2-time);
		time = time2;
		if (_delay >= 0) {
			// TODO : This is really bad ==> fast to code and debug but not optimum at all ... use condition instead ...
			ethread::sleepMilliSeconds((10));
		}
	}
	return false;
}

void ethread::Promise::andThen(etk::Function<void()> _action) {
	ethread::UniqueLock lock(m_mutex);
	m_callback = etk::move(_action);
	if (m_isFinished == true) {
		m_callback();
	}
}

