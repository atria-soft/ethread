/** @file
 * @author Edouard DUPIN
 * @copyright 2014, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>

namespace ethread {
	int32_t getLogId();
};

#define ETHREAD_BASE(info,data) ELOG_BASE(ethread::getLogId(),info,data)

#define ETHREAD_PRINT(data)         ETHREAD_BASE(-1, data)
#define ETHREAD_CRITICAL(data)      ETHREAD_BASE(1, data)
#define ETHREAD_ERROR(data)         ETHREAD_BASE(2, data)
#define ETHREAD_WARNING(data)       ETHREAD_BASE(3, data)
#ifdef DEBUG
	#define ETHREAD_INFO(data)          ETHREAD_BASE(4, data)
	#define ETHREAD_DEBUG(data)         ETHREAD_BASE(5, data)
	#define ETHREAD_VERBOSE(data)       ETHREAD_BASE(6, data)
	#define ETHREAD_TODO(data)          ETHREAD_BASE(4, "TODO : " << data)
#else
	#define ETHREAD_INFO(data)          do { } while(false)
	#define ETHREAD_DEBUG(data)         do { } while(false)
	#define ETHREAD_VERBOSE(data)       do { } while(false)
	#define ETHREAD_TODO(data)          do { } while(false)
#endif

#define ETHREAD_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			ETHREAD_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)


