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

#ifndef ORNG_RNG_XORSHIFT128PLUS_H
# define ORNG_RNG_XORSHIFT128PLUS_H

# include "php.h"

extern PHPAPI zend_class_entry *orng_ce_ORNG_XorShift128Plus;

typedef struct _orng_ORNG_XorShift128Plus_obj {
	uint64_t s[2];
	zend_object std;
} orng_ORNG_XorShift128Plus_obj;

static inline orng_ORNG_XorShift128Plus_obj *orng_ORNG_XorShift128Plus_from_obj(zend_object *obj) {
	return (orng_ORNG_XorShift128Plus_obj*)((char*)(obj) - XtOffsetOf(orng_ORNG_XorShift128Plus_obj, std));
}

# define Z_ORNG_ORNG_XorShift128Plus_P(zval) orng_ORNG_XorShift128Plus_from_obj(Z_OBJ_P(zval))

PHP_METHOD(ORNG_XorShift128Plus, __construct);
PHP_METHOD(ORNG_XorShift128Plus, next);
PHP_METHOD(ORNG_XorShift128Plus, range);

PHP_MINIT_FUNCTION(orng_rng_xorshift128plus);

#endif
