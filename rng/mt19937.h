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

#ifndef ORNG_RNG_MT19937_H
# define ORNG_RNG_MT19937_H

# include "php.h"

extern PHPAPI zend_class_entry *orng_ce_ORNG_MT19937;
extern PHPAPI zend_class_entry *orng_ce_ORNG_MT19937PHP;

typedef struct _orng_ORNG_MT19937_obj {
	uint32_t state[624+1];
	uint32_t *next;
	int left;
	zend_long mode;
	zend_object std;
} orng_ORNG_MT19937_obj;

static inline orng_ORNG_MT19937_obj *orng_ORNG_MT19937_from_obj(zend_object *obj) {
	return (orng_ORNG_MT19937_obj*)((char*)(obj) - XtOffsetOf(orng_ORNG_MT19937_obj, std));
}

# define Z_ORNG_ORNG_MT19937_P(zval) orng_ORNG_MT19937_from_obj(Z_OBJ_P(zval))

# define ORNG_RNG_MT19937_MT_RAND_MAX ((zend_long) (0x7FFFFFFF))

# define ORNG_RNG_MT19937_RAND_RANGE_BADSCALING(__n, __min, __max, __tmax) \
	(__n) = (__min) + (zend_long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))

# define ORNG_RNG_MT19937_MODE_NORMAL      0
# define ORNG_RNG_MT19937_MODE_PHP         1

# define ORNG_RNG_MT19937_N                (624)
# define ORNG_RNG_MT19937_M                (397)
# define ORNG_RNG_MT19937_hiBit(u)         ((u) & 0x80000000U)
# define ORNG_RNG_MT19937_loBit(u)         ((u) & 0x00000001U)
# define ORNG_RNG_MT19937_loBits(u)        ((u) & 0x7FFFFFFFU)
# define ORNG_RNG_MT19937_MixBits(u, v)    (ORNG_RNG_MT19937_hiBit(u)|ORNG_RNG_MT19937_loBits(v))

# define ORNG_RNG_MT19937_twist(m,u,v)     (m ^ (ORNG_RNG_MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(ORNG_RNG_MT19937_loBit(v))) & 0x9908b0dfU))
# define ORNG_RNG_MT19937_twist_php(m,u,v) (m ^ (ORNG_RNG_MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(ORNG_RNG_MT19937_loBit(u))) & 0x9908b0dfU))

PHP_METHOD(ORNG_MT19937, __construct);
PHP_METHOD(ORNG_MT19937, next);
PHP_METHOD(ORNG_MT19937, range);

PHP_MINIT_FUNCTION(orng_rng_mt19937);

#endif
