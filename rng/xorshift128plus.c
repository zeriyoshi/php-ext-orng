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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"

#include "../orng_compat.h"
#include "../php_orng.h"

#include "rnginterface.h"

#include "xorshift128plus.h"
#include "xorshift128plus_arginfo.h"

PHPAPI zend_class_entry *orng_ce_ORNG_XorShift128Plus;

static zend_object_handlers orng_object_handlers_ORNG_XorShift128Plus;

static uint64_t orng_ORNG_XorShift128Plus_splitmix64_next(uint64_t *seed)
{
	uint64_t r;
	r = (*seed += 0x9e3779b97f4a7c15);
	r = (r ^ (r >> 30)) * 0xbf58476d1ce4e5b9;
	r = (r ^ (r >> 27)) * 0x94d049bb133111eb;
	return r ^ (r >> 31);
}

static zend_object *orng_ORNG_XorShift128Plus_new(zend_class_entry *ce)
{
	orng_ORNG_XorShift128Plus_obj *obj = (orng_ORNG_XorShift128Plus_obj*)ecalloc(1, sizeof(orng_ORNG_XorShift128Plus_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_ORNG_XorShift128Plus;
	return &obj->std;
}

static uint64_t orng_ORNG_XorShift128Plus_next64(orng_ORNG_XorShift128Plus_obj *obj)
{
	uint64_t s0, s1, r;

	s1 = obj->s[0];
	s0 = obj->s[1];
	r = s0 + s1;
	obj->s[0] = s0;
	s1 ^= s1 << 23;
	obj->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

	return r;
}

PHPAPI zend_long orng_ORNG_XorShift128Plus_next(orng_ORNG_XorShift128Plus_obj *obj)
{
	return (orng_ORNG_XorShift128Plus_next64(obj) >> 1);
}

static uint32_t orng_ORNG_XorShift128Plus_rand_range32(orng_ORNG_XorShift128Plus_obj *obj, uint32_t umax)
{
	uint32_t r, l;

	r = orng_ORNG_XorShift128Plus_next(obj);

	if (UNEXPECTED(umax == UINT32_MAX)) {
		return r;
	}

	umax++;

	if ((umax & (umax - 1)) == 0) {
		return r & (umax - 1);
	}

	l = UINT32_MAX - (UINT32_MAX % umax) - 1;

	while (UNEXPECTED(r > l)) {
		r = orng_ORNG_XorShift128Plus_next(obj);
	}

	return r % umax;
}

static uint64_t orng_ORNG_XorShift128Plus_rand_range64(orng_ORNG_XorShift128Plus_obj *obj, uint32_t umax)
{
	uint64_t r, l;

	r = orng_ORNG_XorShift128Plus_next64(obj);

	if (UNEXPECTED(umax == UINT64_MAX)) {
		return r;
	}

	umax++;

	if ((umax & (umax - 1)) == 0) {
		return r & (umax - 1);
	}

	l = UINT64_MAX - (UINT64_MAX % umax) - 1;

	while (UNEXPECTED(r > l)) {
		r = orng_ORNG_XorShift128Plus_next64(obj);
	}

	return r % umax;
}

/* {{{ \ORNG\XorShift128Plus::__construct(int $seed) */
PHP_METHOD(ORNG_XorShift128Plus, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	orng_ORNG_XorShift128Plus_obj *obj = Z_ORNG_ORNG_XorShift128Plus_P(getThis());

	if (seed == 0) {
		seed = 1;
	}

	obj->s[0] = orng_ORNG_XorShift128Plus_splitmix64_next(&seed);
	obj->s[1] = orng_ORNG_XorShift128Plus_splitmix64_next(&seed);
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::next(): int */
PHP_METHOD(ORNG_XorShift128Plus, next)
{
	orng_ORNG_XorShift128Plus_obj *obj = Z_ORNG_ORNG_XorShift128Plus_P(getThis());
	RETURN_LONG(orng_ORNG_XorShift128Plus_next(obj));
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::range(int $min, int $max): int */
PHP_METHOD(ORNG_XorShift128Plus, range)
{
	zend_long min, max;
	zend_ulong umax;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_MAX_SMALLER_THAN_MIN();
	}

	orng_ORNG_XorShift128Plus_obj *obj = Z_ORNG_ORNG_XorShift128Plus_P(getThis());

	umax = max - min;

#if ZEND_ULONG_MAX > UINT32_MAX
	if (umax > UINT32_MAX) {
		RETURN_LONG((zend_long) (orng_ORNG_XorShift128Plus_rand_range64(obj, umax) + min));
	}
#endif

	RETURN_LONG((zend_long) (orng_ORNG_XorShift128Plus_rand_range32(obj, umax) + min));
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_xorshift128plus)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(XorShift128Plus), class_ORNG_XorShift128Plus_methods);
	orng_ce_ORNG_XorShift128Plus = zend_register_internal_class(&ce);
	zend_class_implements(orng_ce_ORNG_XorShift128Plus, 1, orng_ce_ORNG_RNGInterface);
	orng_ce_ORNG_XorShift128Plus->create_object = orng_ORNG_XorShift128Plus_new;
	memcpy(&orng_object_handlers_ORNG_XorShift128Plus, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	orng_object_handlers_ORNG_XorShift128Plus.offset = XtOffsetOf(orng_ORNG_XorShift128Plus_obj, std);
	orng_object_handlers_ORNG_XorShift128Plus.clone_obj = NULL; //FIXME

	return SUCCESS;
}
