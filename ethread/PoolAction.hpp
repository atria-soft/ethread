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
#include <ememory/memory.hpp>

namespace ethread {
	class PoolAction {
		private:
			uint64_t m_currentPoolId;
			ememory::SharedPtr<ethread::Promise> m_promise;
			std::function<void()> m_call;
		public:
			PoolAction(uint64_t _currentPoolId, ememory::SharedPtr<ethread::Promise> _promise, std::function<void()> _call);
			uint64_t getPoolId() const;
			void call();
	};
}
