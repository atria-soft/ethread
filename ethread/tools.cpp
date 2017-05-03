/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ethread/tools.hpp>
#include <mutex>
#include <map>

static std::mutex g_lock;
static std::map<uint32_t, std::string>& getThreadList() {
	static std::map<uint32_t, std::string> g_val;
	return g_val;
}

static uint32_t getThreadHumanId(std::thread::id _id) {
	uint32_t out = 0;
	uint64_t iddd = std::hash<std::thread::id>()(_id);
	g_lock.lock();
	static std::map<uint64_t, uint32_t> g_list;
	std::map<uint64_t, uint32_t>::iterator it = g_list.find(iddd);
	if (it == g_list.end()) {
		// attribute new ID :
		static uint32_t tmpId = 0;
		g_list.insert(std::pair<uint64_t, uint32_t>(iddd,tmpId));
		out = tmpId;
		tmpId++;
	} else {
		out = it->second;
	}
	g_lock.unlock();
	return out;
	
}

static std::string getThreadName(std::thread::id _id) {
	std::map<uint32_t,std::string>& list = getThreadList();
	uint32_t threadID = getThreadHumanId(_id);
	std::string out;
	g_lock.lock();
	std::map<uint32_t,std::string>::iterator it = list.find(threadID);
	if (it != list.end()) {
		out = it->second;
	}
	g_lock.unlock();
	return out;
}

static void setThreadName(std::thread* _thread, const std::string& _name) {
	std::map<uint32_t,std::string>& list = getThreadList();
	uint32_t threadID = ethread::getId();
	g_lock.lock();
	std::map<uint32_t,std::string>::iterator it = list.find(threadID);
	if (it == list.end()) {
		list.insert(std::pair<uint32_t, std::string>(threadID, _name));
	} else {
		it->second = _name;
	}
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
		std::string name = _name;
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

void ethread::setName(const std::string& _name) {
	setThreadName(nullptr, _name);
}

void ethread::setName(std::thread& _thread, const std::string& _name) {
	setThreadName(&_thread, _name);
}

std::string ethread::getName() {
	return getThreadName(std::this_thread::get_id());
}

std::string ethread::getName(std::thread& _thread) {
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

void ethread::metadataSet(const std::string& _key, uint64_t _value) {
	
}

void ethread::metadataRemove(const std::string& _key) {
	
}

uint64_t ethread::metadataGetU64(const std::string& _key) {
	return 0;
}