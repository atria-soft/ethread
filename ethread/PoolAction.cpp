/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ethread/PoolAction.hpp>
#include "debug.hpp"

ethread::PoolAction::PoolAction(uint64_t _currentPoolId, ememory::SharedPtr<ethread::Promise> _promise, etk::Function<void()> _call) :
  m_currentPoolId(_currentPoolId),
  m_promise(_promise),
  m_call(etk::move(_call)) {
	
}

uint64_t ethread::PoolAction::getPoolId() const {
	return m_currentPoolId;
}

void ethread::PoolAction::call() {
	if (m_call == nullptr) {
		return;
	}
	if (m_call != nullptr) {
		m_call();
	}
	if (m_promise != nullptr) {
		m_promise->finish();
	}
}

