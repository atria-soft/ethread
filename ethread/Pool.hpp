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
	class Pool {
		private:
			std::mutex m_mutex; //!< global add and release some thread
			std::vector<ememory::SharedPtr<ethread::PoolExecutor>> m_listThread; //!< Thread pool
			std::vector<ememory::SharedPtr<ethread::PoolAction>> m_listActions; //!< Thread pool
			std::vector<uint64_t> m_listIdPool; //!< Thread pool
			uint32_t m_lastTrandId; //!< to group the action in a single thread
		public:
			Pool(uint16_t _numberOfThread);
			~Pool();
			uint32_t createGroupId();
			// Execte in a group != of 0 request ordering the action in a single thread (same as a trand ...)
			ethread::Future async(std::function<void()>, uint64_t _executionInGroupId=0); //!< execute an action in the thread pool...
			ememory::SharedPtr<ethread::PoolAction> getAction();
			void releaseId(uint64_t _id);
			void stop();
			void join();
	};
}
