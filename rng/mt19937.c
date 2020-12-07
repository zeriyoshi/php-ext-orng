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

#include "mt19937.h"
#include "mt19937_arginfo.h"
// Note: "mt19937php_arghinfo.h" is not required.

PHPAPI zend_class_entry *orng_ce_ORNG_MT19937;
PHPAPI zend_class_entry *orng_ce_ORNG_MT19937PHP;

static zend_object_handlers orng_object_handlers_ORNG_MT19937;

static zend_object *orng_ORNG_MT19937_new(zend_class_entry *ce)
{
	orng_ORNG_MT19937_obj *obj = (orng_ORNG_MT19937_obj*)ecalloc(1, sizeof(orng_ORNG_MT19937_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_ORNG_MT19937;
	obj->mode = ORNG_RNG_MT19937_MODE_NORMAL;
	return &obj->std;
}

static zend_object *orng_ORNG_MT19937PHP_new(zend_class_entry *ce)
{
	orng_ORNG_MT19937_obj *obj = (orng_ORNG_MT19937_obj*)ecalloc(1, sizeof(orng_ORNG_MT19937_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_ORNG_MT19937;
	obj->mode = ORNG_RNG_MT19937_MODE_PHP;
	return &obj->std;
}

static inline void orng_ORNG_MT19937_initialize(uint32_t seed, uint32_t *state)
{
	register uint32_t *s = state;
	register uint32_t *r = state;
	register int i = 1;

	*s++ = seed & 0xffffffffU;
	for( ; i < ORNG_RNG_MT19937_N; ++i ) {
		*s++ = ( 1812433253U * ( *r ^ (*r >> 30) ) + i ) & 0xffffffffU;
		r++;
	}
}

static inline void orng_ORNG_MT19937_reload(orng_ORNG_MT19937_obj *obj)
{
	register uint32_t *s = obj->state;
	register uint32_t *p = s;
	register int i;

	if (obj->mode == ORNG_RNG_MT19937_MODE_NORMAL) {
		for (i = ORNG_RNG_MT19937_N - ORNG_RNG_MT19937_M; i--; ++p)
			*p = ORNG_RNG_MT19937_twist(p[ORNG_RNG_MT19937_M], p[0], p[1]);
		for (i = ORNG_RNG_MT19937_M; --i; ++p)
			*p = ORNG_RNG_MT19937_twist(p[ORNG_RNG_MT19937_M-ORNG_RNG_MT19937_N], p[0], p[1]);
		*p = ORNG_RNG_MT19937_twist(p[ORNG_RNG_MT19937_M-ORNG_RNG_MT19937_N], p[0], s[0]);
	}
	else {
		for (i = ORNG_RNG_MT19937_N - ORNG_RNG_MT19937_M; i--; ++p)
			*p = ORNG_RNG_MT19937_twist_php(p[ORNG_RNG_MT19937_M], p[0], p[1]);
		for (i = ORNG_RNG_MT19937_M; --i; ++p)
			*p = ORNG_RNG_MT19937_twist_php(p[ORNG_RNG_MT19937_M-ORNG_RNG_MT19937_N], p[0], p[1]);
		*p = ORNG_RNG_MT19937_twist_php(p[ORNG_RNG_MT19937_M-ORNG_RNG_MT19937_N], p[0], s[0]);
	}
	obj->left = ORNG_RNG_MT19937_N;
	obj->next = s;
}

PHPAPI zend_long orng_ORNG_MT19937_next(orng_ORNG_MT19937_obj *obj)
{
	register uint32_t s1;

	if (obj->left == 0) {
		orng_ORNG_MT19937_reload(obj);
	}
	--obj->left;

	s1 = *obj->next++;
	s1 ^= (s1 >> 11);
	s1 ^= (s1 <<  7) & 0x9d2c5680U;
	s1 ^= (s1 << 15) & 0xefc60000U;
	return ( s1 ^ (s1 >> 18) );
}

static uint32_t orng_ORNG_MT19937_rand_range32(orng_ORNG_MT19937_obj *obj, uint32_t umax)
{
	uint32_t r, l;

	r = orng_ORNG_MT19937_next(obj);

	if (UNEXPECTED(umax == UINT32_MAX)) {
		return r;
	}

	umax++;

	if ((umax & (umax - 1)) == 0) {
		return r & (umax - 1);
	}

	l = UINT32_MAX - (UINT32_MAX % umax) - 1;

	while (UNEXPECTED(r > l)) {
		r = orng_ORNG_MT19937_next(obj);
	}

	return r % umax;
}

static uint64_t orng_ORNG_MT19937_rand_range64(orng_ORNG_MT19937_obj *obj, uint32_t umax)
{
	uint64_t r, l;

	r = orng_ORNG_MT19937_next(obj);
	r = (r << 32) | orng_ORNG_MT19937_next(obj);

	if (UNEXPECTED(umax == UINT64_MAX)) {
		return r;
	}

	umax++;

	if ((umax & (umax - 1)) == 0) {
		return r & (umax - 1);
	}

	l = UINT64_MAX - (UINT64_MAX % umax) - 1;

	while (UNEXPECTED(r > l)) {
		r = orng_ORNG_MT19937_next(obj);
		r = (r << 32) | orng_ORNG_MT19937_next(obj);
	}

	return r % umax;
}

/* {{{ \ORNG\MT19937::__construct(int $seed) */
PHP_METHOD(ORNG_MT19937, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	orng_ORNG_MT19937_obj *obj = Z_ORNG_ORNG_MT19937_P(getThis());
	orng_ORNG_MT19937_initialize(seed, obj->state);
}
/* }}} */

/* {{{ \ORNG\MT19937::next(): int */
PHP_METHOD(ORNG_MT19937, next)
{
	orng_ORNG_MT19937_obj *obj = Z_ORNG_ORNG_MT19937_P(getThis());
	RETURN_LONG(orng_ORNG_MT19937_next(obj) >> 1);
}
/* }}} */

/* {{{ \ORNG\MT19937::range(int $min, int $max): int */
PHP_METHOD(ORNG_MT19937, range)
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

	orng_ORNG_MT19937_obj *obj = Z_ORNG_ORNG_MT19937_P(getThis());

	umax = max - min;

#if ZEND_ULONG_MAX > UINT32_MAX
	if (umax > UINT32_MAX) {
		RETURN_LONG((zend_long) (orng_ORNG_MT19937_rand_range64(obj, umax) + min));
	}
#endif

	RETURN_LONG((zend_long) (orng_ORNG_MT19937_rand_range32(obj, umax) + min));
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_mt19937)
{
	zend_class_entry ce, ce2;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(MT19937), class_ORNG_MT19937_methods);
	orng_ce_ORNG_MT19937 = zend_register_internal_class(&ce);
	zend_class_implements(orng_ce_ORNG_MT19937, 1, orng_ce_ORNG_RNGInterface);
	orng_ce_ORNG_MT19937->create_object = orng_ORNG_MT19937_new;
	memcpy(&orng_object_handlers_ORNG_MT19937, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	orng_object_handlers_ORNG_MT19937.offset = XtOffsetOf(orng_ORNG_MT19937_obj, std);
	orng_object_handlers_ORNG_MT19937.clone_obj = NULL; //FIXME

	INIT_CLASS_ENTRY(ce2, ORNG_RNG_FQN(MT19937PHP), class_ORNG_MT19937_methods);
	orng_ce_ORNG_MT19937PHP = zend_register_internal_class(&ce2);
	zend_class_implements(orng_ce_ORNG_MT19937PHP, 1, orng_ce_ORNG_RNGInterface);
	orng_ce_ORNG_MT19937PHP->create_object = orng_ORNG_MT19937PHP_new;

	return SUCCESS;
}
