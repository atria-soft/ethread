/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <ethread/Future.hpp>
#include <ememory/memory.hpp>

namespace ethread {
	/**
	 * @brief A pool Action is a link between an action to do (the call) and a group of excecution (poolId) and the result store in a promise.
	 */
	class PoolAction {
		private:
			uint64_t m_currentPoolId; //!< execution group Id requested
			ememory::SharedPtr<ethread::Promise> m_promise; //!< Return promise of the action
			std::function<void()> m_call; //!< Action to do ...
		public:
			/**
			 * @brief Contuctor of a simple action
			 * @param[in] _currentPoolId Id Of the action might be process inside
			 * @param[in] _promise Promise to call when action is done
			 * @param[in] _call Action to do (callable object)
			 */
			PoolAction(uint64_t _currentPoolId, ememory::SharedPtr<ethread::Promise> _promise, std::function<void()> _call);
			/**
			 * @brief Get the Pool id of the Action
			 * @return The pool id of this action (0 for no request)
			 */
			uint64_t getPoolId() const;
			/**
			 * @brief Call the action to do (real action will be done)
			 */
			void call();
	};
}
