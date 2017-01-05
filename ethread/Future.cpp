/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include "debug.hpp"
#include <ethread/Future.hpp>

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
