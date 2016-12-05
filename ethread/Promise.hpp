/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <mutex>
#include <thread>
#include <echrono/Duration.hpp>

namespace ethread {
	class Promise {
		private:
			std::mutex m_mutex;
			std::function<void()> m_callback;
			bool m_isFinished;
		public:
			Promise();
			void finish();
			bool isFinished();
			bool wait(echrono::Duration _delay=echrono::seconds(2));
			void andThen(std::function<void()> _action);
	};
}
