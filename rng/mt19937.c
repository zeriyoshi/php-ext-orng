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

#include "../orng_util.h"
#include "../orng_compat.h"
#include "../php_orng.h"

#include "rnginterface.h"

#include "mt19937.h"
#include "mt19937_arginfo.h"
// Note: "mt19937php_arghinfo.h" is not required.

PHPAPI zend_class_entry *ce_ORNG_MT19937;
PHPAPI zend_class_entry *ce_ORNG_MT19937PHP;

static zend_object_handlers oh_MT19937;

static inline void internal_mt_initialize(uint32_t seed, uint32_t *state)
{
	register uint32_t *s = state;
	register uint32_t *r = state;
	register int i = 1;

	*s++ = seed & 0xffffffffU;
	for( ; i < ORNG_MT19937_N; ++i ) {
		*s++ = ( 1812433253U * ( *r ^ (*r >> 30) ) + i ) & 0xffffffffU;
		r++;
	}
}

static inline void internal_mt_reload(ORNG_MT19937_obj *obj)
{
	register uint32_t *s = obj->state;
	register uint32_t *p = s;
	register int i;

	if (obj->mode == ORNG_MT19937_MODE_NORMAL) {
		for (i = ORNG_MT19937_N - ORNG_MT19937_M; i--; ++p)
			*p = ORNG_MT19937_twist(p[ORNG_MT19937_M], p[0], p[1]);
		for (i = ORNG_MT19937_M; --i; ++p)
			*p = ORNG_MT19937_twist(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], p[1]);
		*p = ORNG_MT19937_twist(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], s[0]);
	}
	else {
		for (i = ORNG_MT19937_N - ORNG_MT19937_M; i--; ++p)
			*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M], p[0], p[1]);
		for (i = ORNG_MT19937_M; --i; ++p)
			*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], p[1]);
		*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], s[0]);
	}
	obj->left = ORNG_MT19937_N;
	obj->next = s;
}

static uint32_t next32(orng_rng_common *c)
{
	register uint32_t s1;

	ORNG_MT19937_obj *obj = ((ORNG_MT19937_obj*)c->obj);

	if (obj->left == 0) {
		internal_mt_reload(obj);
	}
	--obj->left;

	s1 = *obj->next++;
	s1 ^= (s1 >> 11);
	s1 ^= (s1 <<  7) & 0x9d2c5680U;
	s1 ^= (s1 << 15) & 0xefc60000U;
	return ( s1 ^ (s1 >> 18) );
}

static zend_object *create_object(zend_class_entry *ce)
{
	ORNG_MT19937_obj *obj = (ORNG_MT19937_obj*)ecalloc(1, sizeof(ORNG_MT19937_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, NULL, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_MT19937;
	obj->mode = ORNG_MT19937_MODE_NORMAL;
	return &obj->std;
}

static zend_object *create_object_php(zend_class_entry *ce)
{
	ORNG_MT19937_obj *obj = (ORNG_MT19937_obj*)ecalloc(1, sizeof(ORNG_MT19937_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, NULL, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_MT19937;
	obj->mode = ORNG_MT19937_MODE_PHP;
	return &obj->std;
}

static void free_object(zend_object *object)
{
	ORNG_MT19937_obj *obj = ORNG_MT19937_obj_from_zend_object(object);
	zend_object_std_dtor(&obj->std);
	if (obj->common != NULL) {
		efree(obj->common);
	}
}

ORNG_COMPAT_RNG_CLONE_FUNCTION(MT19937)
{
	zend_object *old_obj = ORNG_COMPAT_RNG_CLONE_GET_OBJ();
	zend_object *new_obj = create_object(old_obj->ce);

	zend_objects_clone_members(new_obj, old_obj);

	ORNG_MT19937_obj *old = ORNG_MT19937_obj_from_zend_object(old_obj);
	ORNG_MT19937_obj *new = ORNG_MT19937_obj_from_zend_object(new_obj);

	memcpy(new->state, old->state, sizeof(old->state));
	new->next = old->next;
	new->left = old->left;
	new->mode = old->mode;

	return new_obj;
}

/* {{{ \ORNG\MT19937::__construct(int $seed) */
PHP_METHOD(ORNG_MT19937, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());
	internal_mt_initialize(seed, obj->state);
}
/* }}} */

/* {{{ \ORNG\MT19937::next(): int */
PHP_METHOD(ORNG_MT19937, next)
{
	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	RETURN_LONG(obj->common->next32(obj->common) >> 1);
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

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	if (obj->mode == ORNG_MT19937_MODE_PHP) {
		int64_t n = (int64_t) obj->common->next32(obj->common) >> 1;
		ORNG_MT19937_RAND_RANGE_BADSCALING(n, min, max, ORNG_MT19937_MT_RAND_MAX);
		RETURN_LONG((zend_long) n);
	}

	RETURN_LONG(orng_rng_common_util_range(obj->common, min, max));
}
/* }}} */

/* {{{ \ORNG\MT19937::shuffle(array &$array): bool */
PHP_METHOD(ORNG_MT19937, shuffle)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	orng_rng_common_util_array_data_shuffle(obj->common, array);

	RETURN_TRUE;
}
/* }}} */

/* {{{ \ORNG\ORNG_MT19937::strShuffle(string $string): string */
PHP_METHOD(ORNG_MT19937, strShuffle)
{
	zend_string *arg;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(arg)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	if (Z_STRLEN_P(return_value) > 1) {
		orng_rng_common_util_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_mt19937)
{
	zend_class_entry ce, ce2;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(MT19937), class_ORNG_MT19937_methods);
	ce_ORNG_MT19937 = zend_register_internal_class(&ce);
	zend_class_implements(ce_ORNG_MT19937, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_MT19937->create_object = create_object;
	memcpy(&oh_MT19937, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_MT19937.offset = XtOffsetOf(ORNG_MT19937_obj, std);
	oh_MT19937.clone_obj = ORNG_COMPAT_RNG_CLONE(MT19937);
	oh_MT19937.free_obj = free_object;

	INIT_CLASS_ENTRY(ce2, ORNG_RNG_FQN(MT19937PHP), class_ORNG_MT19937_methods);
	ce_ORNG_MT19937PHP = zend_register_internal_class(&ce2);
	zend_class_implements(ce_ORNG_MT19937PHP, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_MT19937PHP->create_object = create_object_php;

	return SUCCESS;
}
