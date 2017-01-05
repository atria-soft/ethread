/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <mutex>
#include <thread>
#include <ethread/Promise.hpp>
#include <ememory/memory.hpp>

namespace ethread {
	/**
	 * @brief Simple future to process
	 */
	class Future {
		private:
			ememory::SharedPtr<ethread::Promise> m_promise; //!< the assiciated promise that we are waiting and of action
		public:
			/**
			 * @brief Simple Future contructor
			 * @param[in] _promise Associated promise to wait
			 */
			Future(ememory::SharedPtr<ethread::Promise> _promise=nullptr);
			/**
			 * @brief Check if the action is finished
			 * @return true, the action is done, false otherwise
			 */
			bool isFinished();
			/**
			 * @brief Wait some time that the action finished
			 * @param[in] _delay Delay to wait the action is finished
			 * @return true, the action is finished, false, the time-out apear.
			 */
			bool wait(echrono::Duration _delay=echrono::seconds(2));
			/**
			 * @brief Action to do when the action is finished
			 * @param[in] _action New action to do.
			 */
			void andThen(std::function<void()> _action);
	};
}
