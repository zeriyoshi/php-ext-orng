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

#include "xorshift128plus.h"
#include "xorshift128plus_arginfo.h"

PHPAPI zend_class_entry *ce_ORNG_XorShift128Plus;

static zend_object_handlers oh_XorShift128Plus;

static uint64_t internal_splitmix64_next(uint64_t *seed)
{
	uint64_t r;
	r = (*seed += 0x9e3779b97f4a7c15);
	r = (r ^ (r >> 30)) * 0xbf58476d1ce4e5b9;
	r = (r ^ (r >> 27)) * 0x94d049bb133111eb;
	return (r ^ (r >> 31));
}

static uint32_t next32(orng_rng_common *c)
{
	return (uint32_t) (c->next64(c) << 1);
}

static uint64_t next64(orng_rng_common *c)
{
	uint64_t s0, s1, r;

	ORNG_XorShift128Plus_obj *obj = ((ORNG_XorShift128Plus_obj*)c->obj);

	s1 = obj->s[0];
	s0 = obj->s[1];
	r = s0 + s1;
	obj->s[0] = s0;
	s1 ^= s1 << 23;
	obj->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

	return r;
}

static zend_object *create_object(zend_class_entry *ce)
{
	ORNG_XorShift128Plus_obj *obj = (ORNG_XorShift128Plus_obj*)ecalloc(1, sizeof(ORNG_XorShift128Plus_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, next64, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_XorShift128Plus;
	return &obj->std;
}

static void free_object(zend_object *object)
{
	ORNG_XorShift128Plus_obj *obj = ORNG_XorShift128Plus_obj_from_zend_object(object);
	zend_object_std_dtor(&obj->std);
	if (obj->common != NULL) {
		efree(obj->common);
	}
}

ORNG_COMPAT_RNG_CLONE_FUNCTION(XorShift128Plus)
{
	zend_object *old_obj = ORNG_COMPAT_RNG_CLONE_GET_OBJ();
	zend_object *new_obj = create_object(old_obj->ce);

	zend_objects_clone_members(new_obj, old_obj);

	ORNG_XorShift128Plus_obj *old = ORNG_XorShift128Plus_obj_from_zend_object(old_obj);
	ORNG_XorShift128Plus_obj *new = ORNG_XorShift128Plus_obj_from_zend_object(new_obj);

	memcpy(new->s, old->s, sizeof(old->s));

	return new_obj;
}

/* {{{ \ORNG\XorShift128Plus::__construct(int $seed) */
PHP_METHOD(ORNG_XorShift128Plus, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	obj->s[0] = internal_splitmix64_next(&seed);
	obj->s[1] = internal_splitmix64_next(&seed);
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::next(): int */
PHP_METHOD(ORNG_XorShift128Plus, next)
{
	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());
	RETURN_LONG(obj->common->next64(obj->common));
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
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_RANGE();
	}

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	RETURN_LONG(orng_rng_common_util_range(obj->common, min, max));
}
/* }}} */

/* {{{ \ORNG\ORNG_XorShift128Plus::strShuffle(string $string): string */
PHP_METHOD(ORNG_XorShift128Plus, strShuffle)
{
	zend_string *arg;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(arg)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	if (Z_STRLEN_P(return_value) > 1) {
		orng_rng_common_util_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::shuffle(array &$array): bool */
PHP_METHOD(ORNG_XorShift128Plus, shuffle)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	orng_rng_common_util_array_data_shuffle(obj->common, array);

	RETURN_TRUE;
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_xorshift128plus)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(XorShift128Plus), class_ORNG_XorShift128Plus_methods);
	ce_ORNG_XorShift128Plus = zend_register_internal_class(&ce);
	zend_class_implements(ce_ORNG_XorShift128Plus, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_XorShift128Plus->create_object = create_object;
	memcpy(&oh_XorShift128Plus, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_XorShift128Plus.offset = XtOffsetOf(ORNG_XorShift128Plus_obj, std);
	oh_XorShift128Plus.clone_obj = ORNG_COMPAT_RNG_CLONE(XorShift128Plus);
	oh_XorShift128Plus.free_obj = free_object;

	return SUCCESS;
}
