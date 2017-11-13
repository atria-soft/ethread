/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
#include <etk/types.hpp>

#ifdef __TARGET_OS__Windows
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace ethread {
	/**
	 * @brief Generic mutex interface (OS independent)
	 */
	// TODO:  Create a single class parametrable for all mutex ...
	class MutexRecursive {
		private:
			#ifdef __TARGET_OS__Windows
				CRITICAL_SECTION m_mutex;
			#else
				pthread_mutex_t m_mutex;
				pthread_mutexattr_t m_attribute;
			#endif
			#ifdef DEBUG
				uint32_t m_threadThatHaveLock;
			#endif
		public:
			/**
			 * @brief Create a new mutex recursive
			 */
			MutexRecursive();
			/**
			 * @brief Destroy the mutex.
			 */
			~MutexRecursive();
			/**
			 * @brief Lock the mutex (Wait while the mutex is not lock)
			 */
			void lock();
			/**
			 * @brief Try to lock the mutex (exit if mutex is already locked)
			 * @return true The mutex is locked
			 * @return false The mutex is already locked.
			 */
			bool tryLock();
			/**
			 * @brief Unloc the mutex
			 */
			void unLock();
	};
	/**
	 * @brief AutoLock and un-lock when exit fuction.
	 */
	class RecursiveLock {
		private:
			// Keep a reference on the mutex
			ethread::MutexRecursive &m_protect;
			bool m_lock;
		public:
			/**
			 * @brief constructor that automaticly lock the mutex.
			 * @param[in] _protect Mutex to Lock.
			 * @param[in] _notLock Must be lock after by a tryLock.
			 */
			RecursiveLock(ethread::MutexRecursive& _protect, bool _notLock = false) :
			  m_protect(_protect),
			  m_lock(false) {
				if (_notLock == false) {
					m_protect.lock();
					m_lock = true;
				}
			}
			/**
			 * @brief Try to lock the mutex (exit if mutex is already locked)
			 * @return true The mutex is locked
			 * @return false The mutex is already locked.
			 */
			bool tryLock() {
				if (m_protect.tryLock() == true) {
					m_lock = true;
				}
				return m_lock;
			}
			/**
			 * @brief Destructor that Auto Unlock mutex when remove.
			 */
			virtual ~RecursiveLock(){
				if (m_lock == true) {
					m_protect.unLock();
					m_lock = false;
				}
			}
	};
}

