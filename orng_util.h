/*
  +----------------------------------------------------------------------+
  | ORNG - PHP Objective RNG                                             |
  +----------------------------------------------------------------------+
  | Copyright (c) The PHP Group                                          |
  | Copyright (c) Go Kudo                                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Go Kudo <zeriyoshi@gmail.com>                                |
  +----------------------------------------------------------------------+
*/

#ifndef ORNG_UTIL_H
# define ORNG_UTIL_H

# include "php.h"

typedef struct _orng_rng_common orng_rng_common;

typedef struct _orng_rng_common {
	uint32_t (*next32)(orng_rng_common*);
	uint64_t (*next64)(orng_rng_common*);
	void* obj;
} orng_rng_common;

static orng_rng_common* orng_rng_common_initialize(uint32_t (*next32)(orng_rng_common*), uint64_t (*next64)(orng_rng_common*), void *obj)
{
	orng_rng_common *c = (orng_rng_common*)ecalloc(1, sizeof(orng_rng_common));
	c->next32 = next32;
	c->next64 = next64;
	c->obj = obj;

	return c;
}

static uint64_t orng_rng_common_util_next64bynext32(orng_rng_common *common)
{
	uint64_t r;

	r = common->next32(common);
	r = (r << 32) | common->next32(common);

return r;
}

static zend_long orng_rng_common_util_range(orng_rng_common *c, zend_long min, zend_long max)
{
# if ZEND_ULONG_MAX > UINT32_MAX
	uint64_t result, limit;
	uint64_t umax = max - min;

	if (c->next64 != NULL) {
		result = c->next64(c);
	} else {
		result = orng_rng_common_util_next64bynext32(c);
	}

	if (UNEXPECTED(umax == UINT64_MAX)) {
		return (zend_long) (min + result);
	}

	umax++;

	if ((umax & (umax - 1)) == 0) {
		return (zend_long) (min + result & (umax - 1));
	}

	limit = UINT64_MAX - (UINT64_MAX % umax) - 1;

	while (UNEXPECTED(result > limit)) {
		if (c->next64 != NULL) {
			result = c->next64(c);
		} else {
			result = orng_rng_common_util_next64bynext32(c);
		}
	}

	return (zend_long) (min + (result % umax));

# else
	uint32_t result, limit;
	uint32_t umax = max - min;

	result = c->next32(c);

	if (UNEXPECTED(umax == UINT32_MAX)) {
		return (zend_long) (min + result);
	}

	umax++;

	if ((umax & (umax - 1)) == 0) {
		return (zend_long) (min+ (result & (umax - 1)));
	}

	limit = UINT32_MAX - (UINT32_MAX % umax) - 1;

	while (UNEXPECTED(result > limit)) {
		result = c->next32(c);
	}

	return (zend_long) (min + (result % umax));
# endif
}

#endif