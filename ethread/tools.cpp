/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ethread/tools.hpp>
#include <etk/Pair.hpp>
#include <mutex>
#include <etk/Map.hpp>

static std::mutex g_lock;
static etk::Map<uint32_t, etk::String>& getThreadList() {
	static etk::Map<uint32_t, etk::String> g_val;
	return g_val;
}

static uint32_t getThreadHumanId(std::thread::id _id) {
	uint32_t out = 0;
	uint64_t iddd = std::hash<std::thread::id>()(_id);
	g_lock.lock();
	static etk::Map<uint64_t, uint32_t> g_list;
	etk::Map<uint64_t, uint32_t>::Iterator it = g_list.find(iddd);
	if (it == g_list.end()) {
		// attribute new ID :
		static uint32_t tmpId = 0;
		g_list.set(iddd,tmpId);
		out = tmpId;
		tmpId++;
	} else {
		out = it.getValue();
	}
	g_lock.unlock();
	return out;
	
}

static etk::String getThreadName(std::thread::id _id) {
	etk::Map<uint32_t,etk::String>& list = getThreadList();
	uint32_t threadID = getThreadHumanId(_id);
	etk::String out;
	g_lock.lock();
	auto it = list.find(threadID);
	if (it != list.end()) {
		out = it.getValue();
	}
	g_lock.unlock();
	return out;
}

static void setThreadName(std::thread* _thread, const etk::String& _name) {
	etk::Map<uint32_t,etk::String>& list = getThreadList();
	uint32_t threadID = ethread::getId();
	g_lock.lock();
	list.set(threadID, _name);
	g_lock.unlock();
	// try now to set the thread name with Pthread
	#if     defined(__TARGET_OS__Linux) \
	    && !defined(__TARGET_OS__Web)
		pthread_t pthreadID;
		if (_thread == nullptr) {
			pthreadID = pthread_self();
		} else {
			pthreadID = (pthread_t) _thread->native_handle();
		}
		etk::String name = _name;
		if (name.size() > 15) {
			name.resize(15);
		}
		if (pthread_setname_np(pthreadID, name.c_str()) < 0) {
			//TODO: TK_ERROR("Error when setting the Name in the OS thread naming");
		}
	#else
		//TODO: TK_INFO("Can not set the thread name in this OS (local set)");
	#endif
}

uint32_t ethread::getId() {
	return getThreadHumanId(std::this_thread::get_id());
}

uint32_t ethread::getId(std::thread& _thread) {
	return getThreadHumanId(_thread.get_id());
}

void ethread::setName(const etk::String& _name) {
	setThreadName(nullptr, _name);
}

void ethread::setName(std::thread& _thread, const etk::String& _name) {
	setThreadName(&_thread, _name);
}

etk::String ethread::getName() {
	return getThreadName(std::this_thread::get_id());
}

etk::String ethread::getName(std::thread& _thread) {
	return getThreadName(_thread.get_id());
}

#if    defined(__TARGET_OS__Linux) \
    && !defined(__TARGET_OS__Web)
	static void setThreadPriority(pthread_t _threadID, int32_t _priority) {
		int retcode;
		int policy;
		struct sched_param param;
		retcode = pthread_getschedparam(_threadID, &policy, &param);
		if (retcode != 0) {
			//TODO: TK_ERROR("Can not get prioriry " << ((retcode == ESRCH)  ? "WRONG THREAD ID (ESRCH)" :"???") );
			return;
		}
		//TODO: TK_INFO("Try to set the thread proiority at :" << _priority);
		policy = SCHED_OTHER;
		if (_priority < 0) {
			_priority *= -1;
			policy = SCHED_FIFO;
		}
		param.sched_priority = _priority;
		retcode = pthread_setschedparam(_threadID, policy, &param);
		if (retcode != 0) {
			/* TODO: TK_ERROR("Can not set prioriry " << ((retcode == ESRCH)  ? "WRONG THREAD ID (ESRCH)" :
			                                     (retcode == EINVAL) ? "WRONG POLICY (EINVAL)" :
			                                     (retcode == EPERM)  ? "NO PRIVILEGE (EPERM)" :
			                                     "???") );
			*/
		}
	}
	static int32_t getThreadPriority(pthread_t _threadID) {
		int retcode;
		int policy;
		struct sched_param param;
		retcode = pthread_getschedparam(_threadID, &policy, &param);
		if (retcode != 0) {
			//TODO: TK_ERROR("Can not get prioriry " << ((retcode == ESRCH)  ? "WRONG THREAD ID (ESRCH)" : "???") );
			return 20;
		}
		if (policy != SCHED_OTHER) {
			return -param.sched_priority;
		}
		return param.sched_priority;
	}
#endif


void ethread::setPriority(int32_t _priority) {
	#if    defined(__TARGET_OS__Linux) \
	    && !defined(__TARGET_OS__Web)
		pthread_t threadID = pthread_self();
		setThreadPriority(threadID, _priority);
	#endif
}

void ethread::setPriority(std::thread& _thread, int32_t _priority) {
	#if    defined(__TARGET_OS__Linux) \
	    && !defined(__TARGET_OS__Web)
		pthread_t threadID = (pthread_t) _thread.native_handle();
		setThreadPriority(threadID, _priority);
	#endif
}

int32_t ethread::getPriority() {
	#if    defined(__TARGET_OS__Linux) \
	    && !defined(__TARGET_OS__Web)
		pthread_t threadID = pthread_self();
		return getThreadPriority(threadID);
	#else
		return 20;
	#endif
}

int32_t ethread::getPriority(std::thread& _thread) {
	#if    defined(__TARGET_OS__Linux) \
	    && !defined(__TARGET_OS__Web)
		pthread_t threadID = static_cast<pthread_t>(_thread.native_handle());
		return getThreadPriority(threadID);
	#else
		return 20;
	#endif
}

static std::mutex g_localMutex;
static etk::Map<uint32_t, etk::Map<etk::String, uint64_t>> g_listMetaData;

void ethread::metadataSet(const etk::String& _key, uint64_t _value) {
	uint32_t currentThreadId = ethread::getId();
	std::unique_lock<std::mutex> lock(g_localMutex);
	auto it = g_listMetaData.find(currentThreadId);
	if (it != g_listMetaData.end()) {
		it.getValue().set(_key, _value);
	} else {
		etk::Map<etk::String, uint64_t> tmp;
		tmp.set(_key, _value);
		g_listMetaData.set(currentThreadId, tmp);
	}
}

void ethread::metadataRemove(const etk::String& _key) {
	uint32_t currentThreadId = ethread::getId();
	std::unique_lock<std::mutex> lock(g_localMutex);
	etk::Map<uint32_t, etk::Map<etk::String, uint64_t>>::Iterator it = g_listMetaData.find(currentThreadId);
	if (it != g_listMetaData.end()) {
		auto it2 = it.getValue().find(_key);
		if (it2 != it.getValue().end()) {
			it.getValue().erase(it2);
		}
	}
}

uint64_t ethread::metadataGetU64(const etk::String& _key) {
	uint32_t currentThreadId = ethread::getId();
	std::unique_lock<std::mutex> lock(g_localMutex);
	auto it = g_listMetaData.find(currentThreadId);
	if (it != g_listMetaData.end()) {
		auto it2 = it.getValue().find(_key);
		if (it2 != it.getValue().end()) {
			return it2.getValue();
		}
	}
	return 0;
}