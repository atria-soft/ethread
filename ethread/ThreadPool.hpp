/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <echrono/Future.hpp>

namespace ethread {
	class ThreadPool {
		private:
			std::mutex m_lock; //!< global add and release some thread
			std::vector<std::thread> m_listThread; //!< Thread pool
		public:
			ThreadPool(uint32_t _numberOfThread);
			~ThreadPool();
			uint32_t createGroupId();
			// Execte in a group != of 0 request ordering the action in a single thread (same as a trand ...)
			ethread::Future async(std::function<void()>, uint32_t _executionInGroupId=0); //!< execute an Action in the thread pool...
		
	};
}
