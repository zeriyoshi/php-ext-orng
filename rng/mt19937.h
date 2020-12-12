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

#ifndef ORNG_MT19937_H
# define ORNG_MT19937_H

# include "php.h"

# include "../orng_util.h"

extern PHPAPI zend_class_entry *ce_ORNG_MT19937;
extern PHPAPI zend_class_entry *ce_ORNG_MT19937PHP;
extern PHPAPI zend_class_entry *ce_ORNG_MT19937MB;

# define ORNG_MT19937_MT_RAND_MAX ((zend_long) (0x7FFFFFFF))

# define ORNG_MT19937_RAND_RANGE_BADSCALING(__n, __min, __max, __tmax) \
	(__n) = (__min) + (zend_long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))

# define ORNG_MT19937_MODE_NORMAL      0
# define ORNG_MT19937_MODE_PHP         1
# define ORNG_MT19937_MODE_MB          2

# define ORNG_MT19937_N                (624)
# define ORNG_MT19937_M                (397)
# define ORNG_MT19937_hiBit(u)         ((u) & 0x80000000U)
# define ORNG_MT19937_loBit(u)         ((u) & 0x00000001U)
# define ORNG_MT19937_loBits(u)        ((u) & 0x7FFFFFFFU)
# define ORNG_MT19937_MixBits(u, v)    (ORNG_MT19937_hiBit(u)|ORNG_MT19937_loBits(v))

# define ORNG_MT19937_twist(m,u,v)     (m ^ (ORNG_MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(ORNG_MT19937_loBit(v))) & 0x9908b0dfU))
# define ORNG_MT19937_twist_php(m,u,v) (m ^ (ORNG_MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(ORNG_MT19937_loBit(u))) & 0x9908b0dfU))

typedef struct _ORNG_MT19937_obj {
	uint32_t state[ORNG_MT19937_N + 1];
	uint32_t *next;
	int left;
	zend_long mode;
	orng_rng_common *common;
	zend_object std;
} ORNG_MT19937_obj;

static inline ORNG_MT19937_obj *ORNG_MT19937_obj_from_zend_object(zend_object *obj) {
	return (ORNG_MT19937_obj*)((char*)(obj) - XtOffsetOf(ORNG_MT19937_obj, std));
}

# define Z_ORNG_MT19937_P(zval) ORNG_MT19937_obj_from_zend_object(Z_OBJ_P(zval))

PHP_MINIT_FUNCTION(orng_rng_mt19937);

#endif
