/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ethread/Thread.hpp>
#include <ethread/tools.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::Thread);

namespace ethread {
	uint32_t getThreadHumanId(uint64_t _id);
	etk::String getThreadName(uint64_t _id);
	void setThreadName(ethread::Thread* _thread, const etk::String& _name);
}

void* ethread::Thread::threadCallback(void* _userData) {
	ethread::Thread* threadHandle = static_cast<ethread::Thread*>(_userData);
	if (threadHandle != nullptr) {
		threadHandle->threadCall();
	}
	return nullptr;
}


ethread::Thread::Thread(etk::Function<void()>&& _call, const etk::String& _name):
  m_thread(0),
  m_uid(-1),
  m_name(_name),
  m_function(etk::move(_call)) {
	uint32_t iii = ethread::getId();
	pthread_create(&m_thread, nullptr, &ethread::Thread::threadCallback, this);
	m_uid = ethread::getThreadHumanId(uint64_t(m_thread));
	printf("New thread: %d from %d", m_uid, iii);
}

ethread::Thread::~Thread() {
	
}

void ethread::Thread::join() {
	void* ret = nullptr;
	int val = pthread_join(m_thread, &ret);
}

bool ethread::Thread::detach() {
	
	return true;
}

void ethread::Thread::setName(const etk::String& _name) {
	m_name = _name;
	ethread::setThreadName(this, m_name);
}

const etk::String& ethread::Thread::getName() const {
	return m_name;
}

void ethread::Thread::threadCall() {
	if (m_function != nullptr) {
		m_function();
	}
}

uint64_t ethread::Thread::getId() const {
	return m_uid;
}


