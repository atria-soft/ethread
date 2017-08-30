/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
#include <etk/types.h>

#ifdef __TARGET_OS__Windows
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace ethread {
	/**
	 * @brief Generic mutex interface (OS independent)
	 */
	class Mutex {
		private:
			#ifdef __TARGET_OS__Windows
				CRITICAL_SECTION m_mutex;
			#else
				pthread_mutex_t  m_mutex;
			#endif
		public:
			/**
			 * @brief Create a new mutex
			 */
			Mutex();
			/**
			 * @brief Destroy the mutex.
			 */
			~Mutex();
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
	class uniqueLock {
		private:
			// Keep a reference on the mutex
			ethread::Mutex &m_protect;
		public:
			/**
			 * @brief constructor that automaticly lock the mutex.
			 * @param[in] _protect Mutex to Lock.
			 */
			uniqueLock(ethread::Mutex& _protect) :
			  m_protect(_protect) {
				m_protect.lock();
			}
			/**
			 * @brief Destructor that Auto Unlock mutex when remove.
			 */
			virtual ~uniqueLock(){
				m_protect.unLock();
			}
	};
};

#endif
