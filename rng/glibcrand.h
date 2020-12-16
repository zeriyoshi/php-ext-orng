/*
  +----------------------------------------------------------------------+
  | ORNG - PHP Objective RNG                                             |
  +----------------------------------------------------------------------+
  | Copyright (c) The PHP Group                                          |
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

#ifndef ORNG_GLIBCRAND_H
# define ORNG_GLIBCRAND_H

# include "php.h"

# include "../orng_util.h"

extern PHPAPI zend_class_entry *ce_ORNG_GLibCRand;

/* System Rand functions */
# ifndef ORNG_GLIBCRAND_MAX
#  define ORNG_GLIBCRAND_MAX (1<<15)
# endif

/* In ZTS mode we rely on rand_r() so we must use RAND_MAX. */
# if !defined(ZTS) && (defined(HAVE_LRAND48) || defined(HAVE_RANDOM))
#  define ORNG_GLIBCRAND_RAND_MAX 2147483647
# else
#  define ORNG_GLIBCRAND_RAND_MAX RAND_MAX
# endif

# define ORNG_GLIBCRAND_RAND_RANGE(__n, __min, __max, __tmax) \
    (__n) = (__min) + (zend_long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))

# define ORNG_GLIBCRAND_R 344

typedef struct _ORNG_GLibCRand_obj {
	int r[ORNG_GLIBCRAND_R];
	int next;
  orng_rng_common *common;
	zend_object std;
} ORNG_GLibCRand_obj;

static inline ORNG_GLibCRand_obj *ORNG_GLibCRand_obj_from_zend_object(zend_object *obj) {
	return (ORNG_GLibCRand_obj*)((char*)(obj) - XtOffsetOf(ORNG_GLibCRand_obj, std));
}

# define Z_ORNG_GLibCRand_P(zval) ORNG_GLibCRand_obj_from_zend_object(Z_OBJ_P(zval))

PHP_MINIT_FUNCTION(orng_rng_glibcrand);

#endif
