/* orng extension for PHP */

#ifndef PHP_ORNG_H
# define PHP_ORNG_H

extern zend_module_entry orng_module_entry;
# define phpext_orng_ptr &orng_module_entry

PHP_RINIT_FUNCTION(orng);
PHP_RSHUTDOWN_FUNCTION(orng);
PHP_MINIT_FUNCTION(orng);
PHP_MSHUTDOWN_FUNCTION(orng);
PHP_MINFO_FUNCTION(orng);

PHPAPI zend_class_entry *orng_ce_RNGInterface;
PHPAPI zend_class_entry *orng_ce_XorShift128Plus;

typedef struct _php_orng_XorShift128Plus_obj {
	uint64_t s[2];
	zend_object std;
} php_orng_XorShift128Plus_obj;

static inline php_orng_XorShift128Plus_obj *php_orng_XorShift128Plus_from_obj(zend_object *obj) {
	return (php_orng_XorShift128Plus_obj*)((char*)(obj) - XtOffsetOf(php_orng_XorShift128Plus_obj, std));
}

#define Z_ORNG_XorShift128Plus_P(zval) php_orng_XorShift128Plus_from_obj(Z_OBJ_P(zval))

# define PHP_ORNG_VERSION "0.0.0"

# if defined(ZTS) && defined(COMPILE_DL_ORNG)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ORNG_H */
