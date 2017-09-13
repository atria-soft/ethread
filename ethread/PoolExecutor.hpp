/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ethread/Mutex.hpp>
#include <etk/Vector.hpp>
#include <ethread/Thread.hpp>
#include <ethread/Future.hpp>
#include <ethread/PoolAction.hpp>
#include <ethread/Pool.hpp>
#include <ethread/Mutex.hpp>
#include <ethread/Semaphore.hpp>

namespace ethread {
	/**
	 * @brief A pool Executor is a class that execute some PoolAction. it is contituated wit a simple thread that execute some code).
	 */
	class PoolExecutor {
		private: //section to permit to optimize CPU:
			ethread::Mutex m_mutex; //!< protection of the internal data.
			std::condition_variable m_condition; //!< Message system to send event on an other thread.
			bool m_needProcess; //!< Need to do action (no need to wait condition).
			bool m_isWaiting; //!< The executor is waiting to some action to do.
		private:
			ethread::Pool& m_pool; //!< Local reference on the Thread pool that store action to do.
			ememory::SharedPtr<ethread::Thread> m_thread; //!< Local thread to process action.
			bool m_running; //!< Thread is running (not stop).
			ememory::SharedPtr<ethread::PoolAction> m_action; //!< Curent action that is processing.
		public:
			/**
			 * @brief Create a thread executor for the specific pool.
			 * @param[in] _pool Reference on the thread pool.
			 */
			PoolExecutor(ethread::Pool& _pool);
		protected:
			/**
			 * @brief Internal thread callback (for ethread::Thread).
			 */
			void threadCallback();
		public:
			/**
			 * @brief Start the current thread.
			 */
			void start();
			/**
			 * @brief Stop the current thread.
			 */
			void stop();
			/**
			 * @brief Join the current thread.
			 */
			void join();
			/**
			 * @brief Touche the execurot to process some other data.
			 * @return true if the executor is waiting to process something. false otherwise.
			 */
			bool touch();
	};
}
