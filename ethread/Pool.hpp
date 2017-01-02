/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <ethread/Future.hpp>
#include <ethread/PoolAction.hpp>

namespace ethread {
	class PoolExecutor;
	/**
	 * @brief A pool is an association of a list of active thread that excecute some actions
	 */
	class Pool {
		private:
			std::mutex m_mutex; //!< global add and release some thread
			std::vector<ememory::SharedPtr<ethread::PoolExecutor>> m_listThread; //!< Thread pool
			std::vector<ememory::SharedPtr<ethread::PoolAction>> m_listActions; //!< Thread pool
			std::vector<uint64_t> m_listIdPool; //!< Thread pool
			uint32_t m_lastTrandId; //!< to group the action in a single thread
		public:
			/**
			 * @brief Contructor of the threadPool
			 * @param[in] _numberOfThread Basic number of thread
			 */
			Pool(uint16_t _numberOfThread);
			/**
			 * @brief DEstructor (virtualized)
			 */
			virtual ~Pool();
			/**
			 * @brief Create a new group ID to manage a single ID
			 * @return A simple incremented group ID (number 0 to 10 are reserved)
			 */
			uint32_t createGroupId();
			/**
			 * @brief Ad a async action to process later
			 * @param[in] _func Function to execure to process action
			 * @param[in] _executionInGroupId Group in which the process must be run
			 * @return A future on the action done
			 */
			// Execte in a group != of 0 request ordering the action in a single thread (same as a trand ...)
			ethread::Future async(std::function<void()> _func, uint64_t _executionInGroupId=0); //!< execute an action in the thread pool...
		// internal:
			/**
			 * @brief Gan an Action to process
			 * @return A single action to process in a thread
			 */
			ememory::SharedPtr<ethread::PoolAction> getAction();
			/**
			 * @brief Release an action id in the pool of action in progress
			 * @param[in] _id Id to release
			 */
			void releaseId(uint64_t _id);
			/**
			 * @brief Stop all the thread and current actions
			 */
			void stop();
			/**
			 * @brief Join all thread to destroy the pool
			 */
			void join();
	};
}
