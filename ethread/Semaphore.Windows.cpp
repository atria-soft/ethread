/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <etk/types.hpp>
#include <etk/os/Semaphore.hpp>
#include <etk/debug.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ethread::Semaphore);

etk::Semaphore::Semaphore(uint32_t _nbBasicElement, uint32_t _nbMessageMax) {
	// create interface mutex :
	m_semaphore = CreateSemaphore(null, _nbBasicElement, _nbMessageMax, null);
	TK_ASSERT(m_semaphore != 0, "Error creating SEMAPHORE ...");
}


etk::Semaphore::~Semaphore() {
	CloseHandle(m_semaphore);
}

uint32_t etk::Semaphore::getCount() {
	LONG tmpData = 0;
	ReleaseSemaphore(m_semaphore, 0, &tmpData);
	return tmpData;
}

void etk::Semaphore::post() {
	ReleaseSemaphore(m_semaphore, 1, null);
}


void etk::Semaphore::wait() {
	WaitForSingleObject(m_semaphore, INFINITE);
}


bool etk::Semaphore::wait(uint64_t _timeOutInUs) {
	DWORD result = WaitForSingleObject(m_semaphore, _timeOutInUs);
	if (result == WAIT_FAILED) {
		TK_ERROR("Failed to wait for semaphore ");
		return false;
	} else {
		return result == WAIT_OBJECT_0;
	}
}

