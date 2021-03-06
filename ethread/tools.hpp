/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ethread/Thread.hpp>
#include <etk/String.hpp>

/**
 * @brief ethread main namespace
 */
namespace ethread {
	/**
	 * @brief get human readable thread ID. (not the ethread::Thread::getId())
	 * @return the ID of the thread.
	 */
	uint32_t getId();
	/**
	 * @brief get human readable thread ID. (not the ethread::Thread::getId())
	 * @param[in] _thread Thread handle
	 * @return the ID of the thread.
	 */
	uint32_t getId(ethread::Thread& _thread);
	/**
	 * @brief Set the Current thread name
	 * @param[in] _name New name of the thread
	 */
	void setName(const etk::String& _name);
	/**
	 * @brief Set an other thread name
	 * @param[in] _thread Thread handle
	 * @param[in] _name New name of the thread
	 */
	void setName(ethread::Thread& _thread, const etk::String& _name);
	/**
	 * @brief Set the Current thread name
	 * @return The current name of the thread
	 */
	etk::String getName();
	/**
	 * @brief Get an other thread name
	 * @param[in] _thread Thread handle
	 * @return The external thread name of the thread
	 */
	etk::String getName(ethread::Thread& _thread);
	/**
	 * @brief Set the Current thread priority [-20..0] for RT and ]0..50] for normal priority
	 * @param[in] _priority New priority of the thread
	 * @note If your process have not the right to change thread name, it does not work
	 */
	void setPriority(int32_t _priority);
	/**
	 * @brief Set an other thread priority [-20..0] for RT and ]0..50] for normal priority
	 * @param[in] _thread Thread handle
	 * @param[in] _priority New priority of the thread
	 * @note If your process have not the right to change thread name, it does not work
	 */
	void setPriority(ethread::Thread& _thread, int32_t _priority);
	/**
	 * @brief get the Current thread priority [-20..0] for RT and ]0..50] for normal priority
	 * @return current priority of the thread
	 */
	int32_t getPriority();
	/**
	 * @brief Get an other thread priority [-20..0] for RT and ]0..50] for normal priority
	 * @param[in] _thread Thread handle
	 * @return current priority of the thread
	 */
	int32_t getPriority(ethread::Thread& _thread);
	/**
	 * @brief Set an information with a key on the current thread
	 * @param[in] _key key to store the value
	 * @param[in] _value Value to store
	 */
	void metadataSet(const etk::String& _key, uint64_t _value);
	/**
	 * @brief Remove the information with a key on the current thread
	 * @param[in] _key key to remove
	 */
	void metadataRemove(const etk::String& _key);
	/**
	 * @brief get the information with a key on the current thread
	 * @param[in] _key key to store the value
	 * @return the uint 64 value to stored
	 */
	uint64_t metadataGetU64(const etk::String& _key);
	
	void sleepMilliSeconds(uint32_t _timeInMilliSeconds);
}
