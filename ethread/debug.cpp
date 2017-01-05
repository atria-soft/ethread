/** @file
 * @author Edouard DUPIN
 * @copyright 2014, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include "debug.hpp"

int32_t ethread::getLogId() {
	static int32_t g_val = elog::registerInstance("ethread");
	return g_val;
}
