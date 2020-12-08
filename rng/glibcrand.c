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

#include "glibcrand.h"
#include "glibcrand_arginfo.h"

PHPAPI zend_class_entry *orng_ce_ORNG_GLibCRand;

static zend_object_handlers orng_object_handlers_ORNG_GLibCRand;

static zend_object *orng_ORNG_GLibCRand_new(zend_class_entry *ce)
{
	orng_ORNG_GLibCRand_obj *obj = (orng_ORNG_GLibCRand_obj*)ecalloc(1, sizeof(orng_ORNG_GLibCRand_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_ORNG_GLibCRand;
	return &obj->std;
}

ORNG_COMPAT_RNG_CLONE_FUNCTION(GLibCRand)
{
	zend_object *old_obj = ORNG_COMPAT_RNG_CLONE_GET_OBJ();
	zend_object *new_obj = orng_ORNG_GLibCRand_new(old_obj->ce);

	zend_objects_clone_members(new_obj, old_obj);

	orng_ORNG_GLibCRand_obj *old = orng_ORNG_GLibCRand_from_obj(old_obj);
	orng_ORNG_GLibCRand_obj *new = orng_ORNG_GLibCRand_from_obj(new_obj);

	memcpy(new->r, old->r, sizeof(old->r));
	new->next = old->next;

	return new_obj;
}

PHPAPI zend_long orng_ORNG_GLibCRand_next(orng_ORNG_GLibCRand_obj *obj)
{
	zend_long r;

	unsigned int x = obj->r[obj->next % 344] = obj->r[(obj->next + 313) % 344] + obj->r[(obj->next + 341) % 344];
	obj->next = (obj->next + 1) % 344;
	r = (x >> 1);

	return r;
}

/* {{{ \ORNG\GLibCRand::__construct(int $seed) */
PHP_METHOD(ORNG_GLibCRand, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	orng_ORNG_GLibCRand_obj *obj = Z_ORNG_ORNG_GLibCRand_P(getThis());

	if (seed == 0) {
		seed = 1;
	}

	obj->r[0] = seed;
	for (i = 1; i < 31; i++) {
		obj->r[i] = (unsigned int)((16807 * (unsigned long) obj->r[i - 1]) % 2147483647);
	}
	for (i = 31; i < 34; i++) {
		obj->r[i] = obj->r[i - 31];
	}
	for (i = 34; i < 344; i++) {
		obj->r[i] = obj->r[i - 31] + obj->r[i - 3];
	}
	obj->next = 0;
}
/* }}} */

/* {{{ \ORNG\GLibCRand::next(): int */
PHP_METHOD(ORNG_GLibCRand, next)
{
	orng_ORNG_GLibCRand_obj *obj = Z_ORNG_ORNG_GLibCRand_P(getThis());
	RETURN_LONG(orng_ORNG_GLibCRand_next(obj));
}
/* }}} */

/* {{{ \ORNG\GLibCRand::range(int $min, int $max): int */
PHP_METHOD(ORNG_GLibCRand, range)
{
	zend_long min, max, n;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_MAX_SMALLER_THAN_MIN();
	}

	orng_ORNG_GLibCRand_obj *obj = Z_ORNG_ORNG_GLibCRand_P(getThis());

	n = orng_ORNG_GLibCRand_next(obj);
	n = min + (zend_long) ((double) ((double) max - min + 1.0) * (n / (2147483647 + 1.0)));

	RETURN_LONG(n);
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_glibcrand)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(GLibCRand), class_ORNG_GLibCRand_methods);
	orng_ce_ORNG_GLibCRand = zend_register_internal_class(&ce);
	zend_class_implements(orng_ce_ORNG_GLibCRand, 1, orng_ce_ORNG_RNGInterface);
	orng_ce_ORNG_GLibCRand->create_object = orng_ORNG_GLibCRand_new;
	memcpy(&orng_object_handlers_ORNG_GLibCRand, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	orng_object_handlers_ORNG_GLibCRand.offset = XtOffsetOf(orng_ORNG_GLibCRand_obj, std);
	orng_object_handlers_ORNG_GLibCRand.clone_obj = ORNG_COMPAT_RNG_CLONE(GLibCRand);

	return SUCCESS;
}
