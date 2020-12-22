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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"

#include "../orng_compat.h"
#include "../php_orng.h"

#include "rnginterface.h"
#if PHP_VERSION_ID >= 80000
# include "rnginterface_arginfo.h"
#else
# include "rnginterface_arginfo_7.h"
#endif

PHP_ORNG_API zend_class_entry *orng_ce_ORNG_RNGInterface;

PHP_MINIT_FUNCTION(orng_rng_rnginterface)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(RNGInterface), class_ORNG_RNGInterface_methods);
	orng_ce_ORNG_RNGInterface = zend_register_internal_interface(&ce);

	return SUCCESS;
}
