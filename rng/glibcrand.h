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

#ifndef ORNG_GLIBCRAND_H
# define ORNG_GLIBCRAND_H

# include "php.h"

# include "../orng_util.h"

extern PHPAPI zend_class_entry *ce_ORNG_GLibCRand;

typedef struct _ORNG_GLibCRand_obj {
	int r[344];
	int next;
  orng_rng_common *common;
	zend_object std;
} ORNG_GLibCRand_obj;

static inline ORNG_GLibCRand_obj *ORNG_GLibCRand_obj_from_zend_object(zend_object *obj) {
	return (ORNG_GLibCRand_obj*)((char*)(obj) - XtOffsetOf(ORNG_GLibCRand_obj, std));
}

# define Z_ORNG_GLibCRand_P(zval) ORNG_GLibCRand_obj_from_zend_object(Z_OBJ_P(zval))

PHP_MINIT_FUNCTION(orng_glibcrand);

#endif