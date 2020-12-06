/*
  +----------------------------------------------------------------------+
  | ORNG - PHP Objective RNG Extension                                   |
  +----------------------------------------------------------------------+
  | Copyright (c) zeriyoshi                                              |
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
  | Author: zeriyoshi <zeriyoshi@gmail.com>                              |
  +----------------------------------------------------------------------+
*/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_orng.h"
#include "arginfo_compat.h"
#include "orng_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

static zend_object_handlers orng_object_handlers_GLibCRand;

static zend_object *php_orng_GLibCRand_new(zend_class_entry *ce)
{
	php_orng_GLibCRand_obj *obj = (php_orng_GLibCRand_obj*)ecalloc(1, sizeof(php_orng_GLibCRand_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_GLibCRand;
	return &obj->std;
}

static void php_orng_GLibCRand_destroy_object(zend_object *object)
{
	php_orng_GLibCRand_obj *obj = php_orng_GLibCRand_from_obj(object);
	zend_objects_destroy_object(object);
}

static void php_orng_GLibCRand_object_free_storage(zend_object *object)
{
	php_orng_GLibCRand_obj *obj = php_orng_GLibCRand_from_obj(object);
	zend_object_std_dtor(&obj->std);
}

PHPAPI zend_long php_orng_GLibCRand_next(php_orng_GLibCRand_obj *object)
{
	zend_long ret;

	unsigned int x = object->r[object->next % 344] = object->r[(object->next + 313) % 344] + object->r[(object->next + 341) % 344];
	object->next = (object->next + 1) % 344;
	ret = (x >> 1);

	return ret;
}

/* {{{ \ORNG\GLibCRand::__construct(int $seed) */
PHP_METHOD(ORNG_GLibCRand, __construct)
{
	zend_long seed;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	php_orng_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	unsigned int seed_real = (unsigned int) seed;
	int i;

	if (seed_real == 0) {
		seed_real = 1;
	}

	obj->r[0] = seed_real;
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
	php_orng_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());
	RETURN_LONG(php_orng_GLibCRand_next(obj));
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

	php_orng_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	n = php_orng_GLibCRand_next(obj);
	n = min + (zend_long) ((double) ((double) max - min + 1.0) * (n / (2147483647 + 1.0)));

	RETURN_LONG(n);
}
/* }}} */

static zend_object_handlers orng_object_handlers_XorShift128Plus;

static zend_object *php_orng_XorShift128Plus_new(zend_class_entry *ce)
{
	php_orng_XorShift128Plus_obj *obj = (php_orng_XorShift128Plus_obj*)ecalloc(1, sizeof(php_orng_XorShift128Plus_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_XorShift128Plus;
	return &obj->std;
}

static void php_orng_XorShift128Plus_destroy_object(zend_object *object)
{
	php_orng_XorShift128Plus_obj *obj = php_orng_XorShift128Plus_from_obj(object);
	zend_objects_destroy_object(object);
}

static void php_orng_XorShift128Plus_object_free_storage(zend_object *object)
{
	php_orng_XorShift128Plus_obj *obj = php_orng_XorShift128Plus_from_obj(object);
	zend_object_std_dtor(&obj->std);
}

PHPAPI uint64_t php_orng_XorShift128Plus_splitmix64_next(uint64_t *seed)
{
	uint64_t r;
	r = (*seed += 0x9e3779b97f4a7c15);
	r = (r ^ (r >> 30)) * 0xbf58476d1ce4e5b9;
	r = (r ^ (r >> 27)) * 0x94d049bb133111eb;
	return r ^ (r >> 31);
}

PHPAPI zend_long php_orng_XorShift128Plus_next(php_orng_XorShift128Plus_obj *object)
{
	uint64_t s0, s1, result;

	s1 = object->s[0];
	s0 = object->s[1];
	result = s0 + s1;
	object->s[0] = s0;
	s1 ^= s1 << 23;
	object->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
	return (zend_long) (result >> 1);
}

/* {{{ \ORNG\XorShift128Plus::__construct(int $seed) */
PHP_METHOD(ORNG_XorShift128Plus, __construct)
{
	zend_long seed;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	php_orng_XorShift128Plus_obj *obj = Z_ORNG_XorShift128Plus_P(getThis());

	obj->s[0] = php_orng_XorShift128Plus_splitmix64_next(&seed);
	obj->s[1] = php_orng_XorShift128Plus_splitmix64_next(&seed);
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::next(): int */
PHP_METHOD(ORNG_XorShift128Plus, next)
{
	php_orng_XorShift128Plus_obj *obj = Z_ORNG_XorShift128Plus_P(getThis());
	RETURN_LONG(php_orng_XorShift128Plus_next(obj));
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::range(int $min, int $max): int */
PHP_METHOD(ORNG_XorShift128Plus, range)
{
	zend_long min, max, n;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	php_orng_XorShift128Plus_obj *obj = Z_ORNG_XorShift128Plus_P(getThis());

	// Stolen from: https://github.com/php/php-src/blob/8bb7417ca26c18b18b791c1def2eb7c7b03c74b9/ext/standard/mt_rand.c#L237
	uint64_t result, umax, limit;
	umax = max - min;
	result = php_orng_XorShift128Plus_next(obj);
	result = (result << 32) | php_orng_XorShift128Plus_next(obj);

	if (UNEXPECTED(umax == UINT64_MAX)) {
		RETURN_LONG(result);
	}

	umax++;
	if ((umax & (umax - 1)) == 0) {
		RETURN_LONG(result & (umax - 1));
	}

	limit = UINT64_MAX - (UINT64_MAX % umax) - 1;

	while (UNEXPECTED(result > limit)) {
		result = php_orng_XorShift128Plus_next(obj);
		result = (result << 32) | php_orng_XorShift128Plus_next(obj);
	}

	RETURN_LONG((result % umax) + min);
}
/* }}} */

static zend_object_handlers orng_object_handlers_MT19937;

static zend_object *php_orng_MT19937_new(zend_class_entry *ce)
{
	php_orng_MT19937_obj *obj = (php_orng_MT19937_obj*)ecalloc(1, sizeof(php_orng_MT19937_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_MT19937;
	obj->mode = ORNG_MT19937_MODE_NORMAL;
	return &obj->std;
}

static zend_object *php_orng_MT19937PHP_new(zend_class_entry *ce)
{
	php_orng_MT19937_obj *obj = (php_orng_MT19937_obj*)ecalloc(1, sizeof(php_orng_MT19937_obj) + zend_object_properties_size(ce));
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &orng_object_handlers_MT19937;
	obj->mode = ORNG_MT19937_MODE_PHP;
	return &obj->std;
}

static void php_orng_MT19937_destroy_object(zend_object *object)
{
	php_orng_MT19937_obj *obj = php_orng_MT19937_from_obj(object);
	zend_objects_destroy_object(object);
}

static void php_orng_MT19937_object_free_storage(zend_object *object)
{
	php_orng_MT19937_obj *obj = php_orng_MT19937_from_obj(object);
	zend_object_std_dtor(&obj->std);
}

static inline void php_orng_MT19937_initialize(uint32_t seed, uint32_t *state)
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

static inline void php_orng_MT19937_reload(php_orng_MT19937_obj *object)
{
	register uint32_t *state = object->state;
	register uint32_t *p = state;
	register int i;

	if (object->mode == ORNG_MT19937_MODE_NORMAL) {
		for (i = ORNG_MT19937_N - ORNG_MT19937_M; i--; ++p)
			*p = ORNG_MT19937_twist(p[ORNG_MT19937_M], p[0], p[1]);
		for (i = ORNG_MT19937_M; --i; ++p)
			*p = ORNG_MT19937_twist(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], p[1]);
		*p = ORNG_MT19937_twist(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], state[0]);
	}
	else {
		for (i = ORNG_MT19937_N - ORNG_MT19937_M; i--; ++p)
			*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M], p[0], p[1]);
		for (i = ORNG_MT19937_M; --i; ++p)
			*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], p[1]);
		*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], state[0]);
	}
	object->left = ORNG_MT19937_N;
	object->next = state;
}

PHPAPI zend_long php_orng_MT19937_next(php_orng_MT19937_obj *object)
{
	register uint32_t s1;

	if (object->left == 0) {
		php_orng_MT19937_reload(object);
	}
	--object->left;

	s1 = *object->next++;
	s1 ^= (s1 >> 11);
	s1 ^= (s1 <<  7) & 0x9d2c5680U;
	s1 ^= (s1 << 15) & 0xefc60000U;
	return ( s1 ^ (s1 >> 18) );
}

static uint32_t php_orng_MT19937_rand_range32(php_orng_MT19937_obj *object, uint32_t umax) {
	uint32_t result, limit;

	result = php_orng_MT19937_next(object);

	/* Special case where no modulus is required */
	if (UNEXPECTED(umax == UINT32_MAX)) {
		return result;
	}

	/* Increment the max so the range is inclusive of max */
	umax++;

	/* Powers of two are not biased */
	if ((umax & (umax - 1)) == 0) {
		return result & (umax - 1);
	}

	/* Ceiling under which UINT32_MAX % max == 0 */
	limit = UINT32_MAX - (UINT32_MAX % umax) - 1;

	/* Discard numbers over the limit to avoid modulo bias */
	while (UNEXPECTED(result > limit)) {
		result = php_orng_MT19937_next(object);
	}

	return result % umax;
}

#if ZEND_ULONG_MAX > UINT32_MAX
static uint64_t php_orng_MT19937_rand_range64(php_orng_MT19937_obj *object, uint64_t umax) {
	uint64_t result, limit;

	result = php_orng_MT19937_next(object);
	result = (result << 32) | php_orng_MT19937_next(object);

	/* Special case where no modulus is required */
	if (UNEXPECTED(umax == UINT64_MAX)) {
		return result;
	}

	/* Increment the max so the range is inclusive of max */
	umax++;

	/* Powers of two are not biased */
	if ((umax & (umax - 1)) == 0) {
		return result & (umax - 1);
	}

	/* Ceiling under which UINT64_MAX % max == 0 */
	limit = UINT64_MAX - (UINT64_MAX % umax) - 1;

	/* Discard numbers over the limit to avoid modulo bias */
	while (UNEXPECTED(result > limit)) {
		result = php_orng_MT19937_next(object);
		result = (result << 32) | php_orng_MT19937_next(object);
	}

	return result % umax;
}
#endif

PHPAPI zend_long php_orng_MT19937_rand_range(php_orng_MT19937_obj *object, zend_long min, zend_long max)
{
	zend_ulong umax = max - min;

#if ZEND_ULONG_MAX > UINT32_MAX
	if (umax > UINT32_MAX) {
		return (zend_long) (php_orng_MT19937_rand_range64(object, umax) + min);
	}
#endif

	return (zend_long) (php_orng_MT19937_rand_range32(object, umax) + min);
}

/* {{{ \ORNG\MT19937<PHP>::__construct(int $seed) */
PHP_METHOD(ORNG_MT19937, __construct)
{
	zend_long seed;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	php_orng_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());
	php_orng_MT19937_initialize(seed, obj->state);
}
/* }}} */

/* {{{ \ORNG\MT19937<PHP>::next(): int */
PHP_METHOD(ORNG_MT19937, next)
{
	php_orng_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());
	RETURN_LONG(php_orng_MT19937_next(obj) >> 1);
}
/* }}} */

/* {{{ \ORNG\MT19937<PHP>::range(int $min, int $max): int */
PHP_METHOD(ORNG_MT19937, range)
{
	zend_long min, max;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	php_orng_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	if (obj->mode == ORNG_MT19937_MODE_NORMAL) {
		RETURN_LONG(php_orng_MT19937_rand_range(obj, min, max));
	}

	int64_t n;
	n = (int64_t) php_orng_MT19937_next(obj) >> 1;
	ORNG_MT19937_RAND_RANGE_BADSCALING(n, min, max, ORNG_MT19937_MT_RAND_MAX);

	RETURN_LONG(n);
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(orng)
{
#if defined(ZTS) && defined(COMPILE_DL_ORNG)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(orng)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(orng)
{
	zend_class_entry orng_RNGInterface;
	INIT_CLASS_ENTRY(orng_RNGInterface, "ORNG\\RNGInterface", class_ORNG_RNGInterface_methods);
	orng_ce_RNGInterface = zend_register_internal_interface(&orng_RNGInterface);

	zend_class_entry orng_GLibCRand;
	INIT_CLASS_ENTRY(orng_GLibCRand, "ORNG\\GLibCRand", class_ORNG_GLibCRand_methods);
	orng_ce_GLibCRand = zend_register_internal_class(&orng_GLibCRand);
	zend_class_implements(orng_ce_GLibCRand, 1, orng_ce_RNGInterface);
	orng_ce_GLibCRand->create_object = php_orng_GLibCRand_new;
	memcpy(&orng_object_handlers_GLibCRand, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	orng_object_handlers_GLibCRand.offset = XtOffsetOf(php_orng_GLibCRand_obj, std);
	orng_object_handlers_GLibCRand.clone_obj = NULL;
	orng_object_handlers_GLibCRand.dtor_obj = php_orng_GLibCRand_destroy_object;
	orng_object_handlers_GLibCRand.free_obj = php_orng_GLibCRand_object_free_storage;

	zend_class_entry orng_XorShift128Plus;
	INIT_CLASS_ENTRY(orng_XorShift128Plus, "ORNG\\XorShift128Plus", class_ORNG_XorShift128Plus_methods);
	orng_ce_XorShift128Plus = zend_register_internal_class(&orng_XorShift128Plus);
	zend_class_implements(orng_ce_XorShift128Plus, 1, orng_ce_RNGInterface);
	orng_ce_XorShift128Plus->create_object = php_orng_XorShift128Plus_new;
	memcpy(&orng_object_handlers_XorShift128Plus, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	orng_object_handlers_XorShift128Plus.offset = XtOffsetOf(php_orng_XorShift128Plus_obj, std);
	orng_object_handlers_XorShift128Plus.clone_obj = NULL;
	orng_object_handlers_XorShift128Plus.dtor_obj = php_orng_XorShift128Plus_destroy_object;
	orng_object_handlers_XorShift128Plus.free_obj = php_orng_XorShift128Plus_object_free_storage;

	zend_class_entry orng_MT19937;
	INIT_CLASS_ENTRY(orng_MT19937, "ORNG\\MT19937", class_ORNG_MT19937_methods);
	orng_ce_MT19937 = zend_register_internal_class(&orng_MT19937);
	zend_class_implements(orng_ce_MT19937, 1, orng_ce_RNGInterface);
	orng_ce_MT19937->create_object = php_orng_MT19937_new;
	memcpy(&orng_object_handlers_MT19937, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	orng_object_handlers_MT19937.offset = XtOffsetOf(php_orng_MT19937_obj, std);
	orng_object_handlers_MT19937.clone_obj = NULL;
	orng_object_handlers_MT19937.dtor_obj = php_orng_MT19937_destroy_object;
	orng_object_handlers_MT19937.free_obj = php_orng_MT19937_object_free_storage;

	zend_class_entry orng_MT19937PHP;
	INIT_CLASS_ENTRY(orng_MT19937PHP, "ORNG\\MT19937PHP", class_ORNG_MT19937_methods);
	orng_ce_MT19937PHP = zend_register_internal_class(&orng_MT19937PHP);
	zend_class_implements(orng_ce_MT19937PHP, 1, orng_ce_RNGInterface);
	orng_ce_MT19937PHP->create_object = php_orng_MT19937PHP_new;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(orng)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(orng)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "orng support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ orng_module_entry */
zend_module_entry orng_module_entry = {
	STANDARD_MODULE_HEADER,
	"orng",
	NULL,
	PHP_MINIT(orng),
	PHP_MSHUTDOWN(orng),
	PHP_RINIT(orng),
	PHP_RSHUTDOWN(orng),
	PHP_MINFO(orng),
	PHP_ORNG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ORNG
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(orng)
#endif
