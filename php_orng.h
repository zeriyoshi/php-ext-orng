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

PHPAPI zend_class_entry *orng_ce_GLibCRand;

typedef struct _php_orng_GLibCRand_obj {
	zend_long r[344];
	int next;
	zend_object std;
} php_orng_GLibCRand_obj;

static inline php_orng_GLibCRand_obj *php_orng_GLibCRand_from_obj(zend_object *obj) {
	return (php_orng_GLibCRand_obj*)((char*)(obj) - XtOffsetOf(php_orng_GLibCRand_obj, std));
}

# define Z_ORNG_GLibCRand_P(zval) php_orng_GLibCRand_from_obj(Z_OBJ_P(zval))

PHPAPI zend_class_entry *orng_ce_XorShift128Plus;

typedef struct _php_orng_XorShift128Plus_obj {
	uint64_t s[2];
	zend_object std;
} php_orng_XorShift128Plus_obj;

static inline php_orng_XorShift128Plus_obj *php_orng_XorShift128Plus_from_obj(zend_object *obj) {
	return (php_orng_XorShift128Plus_obj*)((char*)(obj) - XtOffsetOf(php_orng_XorShift128Plus_obj, std));
}

# define Z_ORNG_XorShift128Plus_P(zval) php_orng_XorShift128Plus_from_obj(Z_OBJ_P(zval))

PHPAPI zend_class_entry *orng_ce_MT19937;
PHPAPI zend_class_entry *orng_ce_MT19937PHP;

typedef struct _php_orng_MT19937_obj {
	uint32_t state[624+1];
	uint32_t *next;
	int left;
	zend_long mode;
	zend_object std;
} php_orng_MT19937_obj;

static inline php_orng_MT19937_obj *php_orng_MT19937_from_obj(zend_object *obj) {
	return (php_orng_MT19937_obj*)((char*)(obj) - XtOffsetOf(php_orng_MT19937_obj, std));
}

# define Z_ORNG_MT19937_P(zval) php_orng_MT19937_from_obj(Z_OBJ_P(zval))

# define ORNG_MT19937_MT_RAND_MAX ((zend_long) (0x7FFFFFFF)) /* (1<<31) - 1 */

# define ORNG_MT19937_RAND_RANGE_BADSCALING(__n, __min, __max, __tmax) \
	(__n) = (__min) + (zend_long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))

# define ORNG_MT19937_MODE_NORMAL      0
# define ORNG_MT19937_MODE_PHP         1

# define ORNG_MT19937_N                (624)                                          /* length of state vector */
# define ORNG_MT19937_M                (397)                                          /* a period parameter */
# define ORNG_MT19937_hiBit(u)         ((u) & 0x80000000U)                            /* mask all but highest   bit of u */
# define ORNG_MT19937_loBit(u)         ((u) & 0x00000001U)                            /* mask all but lowest    bit of u */
# define ORNG_MT19937_loBits(u)        ((u) & 0x7FFFFFFFU)                            /* mask     the highest   bit of u */
# define ORNG_MT19937_MixBits(u, v)    (ORNG_MT19937_hiBit(u)|ORNG_MT19937_loBits(v)) /* move hi bit of u to hi bit of v */

# define ORNG_MT19937_twist(m,u,v)     (m ^ (ORNG_MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(ORNG_MT19937_loBit(v))) & 0x9908b0dfU))
# define ORNG_MT19937_twist_php(m,u,v) (m ^ (ORNG_MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(ORNG_MT19937_loBit(u))) & 0x9908b0dfU))

# define PHP_ORNG_VERSION "0.0.0"

# if defined(ZTS) && defined(COMPILE_DL_ORNG)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ORNG_H */
