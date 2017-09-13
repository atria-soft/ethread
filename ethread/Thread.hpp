/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
#include <etk/types.hpp>
#include <etk/Function.hpp>
#include <etk/String.hpp>

#ifdef __TARGET_OS__Windows
	#error TODO ...
#else
	extern "C" {
		#include <pthread.h>
	}
#endif

namespace ethread {
	/**
	 * @brief Generic Thread interface (OS independent)
	 */
	class Thread {
		private:
			#ifdef __TARGET_OS__Windows
				
			#else
				pthread_t m_thread;
				static void* threadCallback(void* _userData);
			#endif
			uint64_t m_uid; //!< unique id of the thread
			etk::String m_name; //!< Name of the thread (do not get it on the system ==> more portable)
			etk::Function<void()> m_function; //!< Function to call every cycle of the thead running
		public:
			Thread(etk::Function<void()>&& _call, const etk::String& _name);
			~Thread();
			void join();
			bool detach();
			void threadCall();
			void setName(const etk::String& _name);
			const etk::String& setName() const;
			uint64_t getId() const;
	};
}

