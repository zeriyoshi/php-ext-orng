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

#ifndef PHP_ORNG_H
# define PHP_ORNG_H

# ifdef PHP_WIN32
#  ifdef PHP_ORNG_EXPORTS
#   define PHP_ORNG_API __declspec(dllexport)
#  else
#   define PHP_ORNG_API __declspec(dllimport)
#  endif
# elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_ORNG_API __attribute__ ((visibility("default")))
# else
#  define PHP_ORNG_API
# endif

# define ORNG_RNG_FQN(__cn) "ORNG\\"#__cn

extern zend_module_entry orng_module_entry;
# define phpext_orng_ptr &orng_module_entry

# define PHP_ORNG_VERSION "0.0.3"

PHP_MINIT_FUNCTION(orng);
PHP_MSHUTDOWN_FUNCTION(orng);
PHP_RINIT_FUNCTION(orng);
PHP_MINFO_FUNCTION(orng);

# if defined(ZTS) && defined(COMPILE_DL_ORNG)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ORNG_H */
