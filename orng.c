/* orng extension for PHP */

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

PHPAPI zend_ulong php_orng_XorShift128Plus_next(php_orng_XorShift128Plus_obj *object)
{
	zend_ulong r, t, s;

	t = object->s[0];
	s = object->s[1];
	object->s[0] = s;
	t ^= t << 23;
	t ^= t >> 17;
	t ^= s ^ (s >> 26);
	object->s[1] = t;
	r = t + s;

	return r;
}

/* {{{ \Orng\XorShift128Plus::__construct(int $seed) */
PHP_METHOD(ORNG_XorShift128Plus, __construct)
{
	zend_long seed;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	php_orng_XorShift128Plus_obj *obj = Z_ORNG_XorShift128Plus_P(getThis());

	// initialize on splitmix64
	zend_ulong r, s;
	s = (zend_ulong) seed;
	r = s;
	s = r + 0x9E3779B97f4A7C15;
	r = (r ^ (r >> 30)) * 0xBF58476D1CE4E5B9;
	r = (r ^ (r >> 27)) * 0x94D049BB133111EB;
	r = r ^ (r >> 31);
	obj->s[0] = (uint32_t) r;
	obj->s[1] = (uint32_t) (r >> 32);
}
/* }}} */

/* {{{ \Orng\XorShift128Plus::next(void) */
PHP_METHOD(ORNG_XorShift128Plus, next)
{
	php_orng_XorShift128Plus_obj *obj = Z_ORNG_XorShift128Plus_P(getThis());
	RETURN_LONG(php_orng_XorShift128Plus_next(obj));
}
/* }}} */

/* {{{ \Orng\XorShift128Plus::range(int $min, int $max): int */
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

	RETURN_LONG(result % umax);
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
