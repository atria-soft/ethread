/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ethread/Future.hpp>

ethread::Promise::Promise():
  m_isFinished(false) {
	
}

bool ethread::Future::isFinished() {
	return m_isFinished;
}

void ethread::Promise::finish() {
	std::function<void()> callback;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_isFinished == true) {
			ETHREAD_ERROR("Request 2 time finishing a Promise ...");
			return;
		}
		m_isFinished = true;
		if (m_callback != nullptr);
			// call callbacks ...
			callback = std::move(m_callback);
		}
	}
	if (callback != nullptr) {
		callback();
	}
}

bool ethread::Promise::wait(echrono::Duration _delay) {
	echrono::Steady time = echrono::Steady::now();
	while (_delay >= 0) {
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (m_isFinished == true) {
				return true;
			}
		}
		echrono::Steady time2 = echrono::Steady::now();
		_delay -= (time2-time);
		time = time2;
		if (_delay >= 0) {
			// TODO : This is really bad ==> fast to code and debug but not optimum at all ... use condition instead ...
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	return false;
}

void ethread::Promise::andThen(std::function<void()> _action) {
	m_callback = std::move(_action);
}


ethread::Future::Future(ememory::SharedPtr<ethread::Promise> _promise):
  m_promise(_promise) {
	
}

bool ethread::Future::isFinished() {
	if (m_promise == nullptr) {
		ETHREAD_ERROR("Promise does not exist...");
		return true;
	}
	return m_promise->isFinished();
}

bool ethread::Future::wait(echrono::Duration _delay) {
	if (m_promise == nullptr) {
		ETHREAD_ERROR("Promise does not exist...");
		return false;
	}
	return m_promise->wait(_delay);
}

void ethread::Future::andThen(std::function<void()> _action) {
	if (m_promise == nullptr) {
		ETHREAD_ERROR("Promise does not exist...");
		return;
	}
	m_promise->andThen(_action);
}
