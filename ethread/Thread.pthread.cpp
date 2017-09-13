/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ethread/Thread.hpp>


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
	pthread_create(&m_thread, nullptr, &ethread::Thread::threadCallback, this);
	/*
	pthread_id_np_t tid;
	pthread_getunique_np(&m_thread, &tid);
	m_uid = uint64_t(tid);
	*/
	m_uid = *(uint64_t*)(m_thread);
}

ethread::Thread::~Thread() {
	
}

void ethread::Thread::join() {
	void* ret = nullptr;
	int val = pthread_join(m_thread, &ret);
}

bool ethread::Thread::detach() {
	
}

void ethread::Thread::setName(const etk::String& _name) {
	m_name = _name;
}

const etk::String& ethread::Thread::setName() const {
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


