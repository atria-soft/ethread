/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <mutex>
#include <thread>
#include <echrono/Duration.hpp>

namespace ethread {
	/**
	 * @brief A promise of the specific ation to do in the thread pool
	 */
	class Promise {
		private:
			std::mutex m_mutex; //!< Simple lock of the interface
			std::function<void()> m_callback; //!< callback to call when processing is ended
			bool m_isFinished; //!< The process of the action has been done
		public:
			/**
			 * @brief Simple empty contructor
			 */
			Promise();
			/**
			 * @brief Call this when the action has been done
			 */
			void finish();
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
