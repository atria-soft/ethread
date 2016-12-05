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
#include <ethread/Pool.hpp>

namespace ethread {
	class PoolExecutor {
		private:
			ethread::Pool& m_pool;
			ememory::SharedPtr<std::thread> m_thread;
			bool m_running;
			uint32_t m_uniqueId;
			ememory::SharedPtr<ethread::PoolAction> m_action;
		public:
			PoolExecutor(ethread::Pool& _pool);
			void threadCallback();
			void start();
			void stop();
			void join();
	};
}
