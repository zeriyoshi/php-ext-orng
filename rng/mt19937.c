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
#include "zend_bitset.h"

#include "../orng_util.h"
#include "../orng_compat.h"
#include "../php_orng.h"

#include "rnginterface.h"

#include "mt19937.h"
#if PHP_VERSION_ID >= 80000
# include "mt19937_arginfo.h"
#else
# include "mt19937_arginfo_7.h"
#endif
// Note: "mt19937php_arghinfo.h" is not required.
// Note: "mt19937mb_arginfo.h" is not required.

PHPAPI zend_class_entry *ce_ORNG_MT19937;
PHPAPI zend_class_entry *ce_ORNG_MT19937PHP;
PHPAPI zend_class_entry *ce_ORNG_MT19937MB;

static zend_object_handlers oh_MT19937;

static inline void internal_mt_initialize(uint32_t seed, uint32_t *state)
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

static inline void internal_mt_reload(ORNG_MT19937_obj *obj)
{
	register uint32_t *s = obj->state;
	register uint32_t *p = s;
	register int i;

	switch (obj->mode) {
		case ORNG_MT19937_MODE_PHP:
			for (i = ORNG_MT19937_N - ORNG_MT19937_M; i--; ++p)
				*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M], p[0], p[1]);
			for (i = ORNG_MT19937_M; --i; ++p)
				*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], p[1]);
			*p = ORNG_MT19937_twist_php(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], s[0]);
			break;
		default:
			for (i = ORNG_MT19937_N - ORNG_MT19937_M; i--; ++p)
				*p = ORNG_MT19937_twist(p[ORNG_MT19937_M], p[0], p[1]);
			for (i = ORNG_MT19937_M; --i; ++p)
				*p = ORNG_MT19937_twist(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], p[1]);
			*p = ORNG_MT19937_twist(p[ORNG_MT19937_M-ORNG_MT19937_N], p[0], s[0]);
			break;
	}
	obj->left = ORNG_MT19937_N;
	obj->next = s;
}

static uint32_t next32(orng_rng_common *c)
{
	register uint32_t s1;

	ORNG_MT19937_obj *obj = ((ORNG_MT19937_obj*)c->obj);

	if (obj->left == 0) {
		internal_mt_reload(obj);
	}
	--obj->left;

	s1 = *obj->next++;
	s1 ^= (s1 >> 11);
	s1 ^= (s1 <<  7) & 0x9d2c5680U;
	s1 ^= (s1 << 15) & 0xefc60000U;
	return ( s1 ^ (s1 >> 18) );
}

static zend_long range(orng_rng_common *c, zend_long min, zend_long max)
{
	return orng_rng_common_util_rand_range(c, min, max);
}

static zend_long range_php(orng_rng_common *c, zend_long min, zend_long max)
{
	int64_t n = (int64_t) c->next32(c) >> 1;
	ORNG_MT19937_RAND_RANGE_BADSCALING(n, min, max, ORNG_MT19937_MT_RAND_MAX);
	return (zend_long) n;
}

/* from upstream: https://github.com/php/php-src/blob/PHP-7.1/ext/standard/mt_rand.c#L214 */
static zend_long range_mb(orng_rng_common *c, zend_long min, zend_long max)
{
	zend_ulong umax = max - min;
	zend_ulong limit;
	zend_ulong result;

	result = c->next32(c);
#if ZEND_ULONG_MAX > UINT32_MAX
	if (umax > UINT32_MAX) {
		result = orng_rng_common_util_next64bynext32(c);
	}
#endif

	/* Special case where no modulus is required */
	if (UNEXPECTED(umax == ZEND_ULONG_MAX)) {
		return (zend_long)result;
	}

	/* Increment the max so the range is inclusive of max */
	umax++;

	/* Powers of two are not biased */
	if (EXPECTED((umax & (umax - 1)) != 0)) {
		/* Ceiling under which ZEND_LONG_MAX % max == 0 */
		limit = ZEND_ULONG_MAX - (ZEND_ULONG_MAX % umax) - 1;

		/* Discard numbers over the limit to avoid modulo bias */
		while (UNEXPECTED(result > limit)) {
#if ZEND_ULONG_MAX > UINT32_MAX
			if (umax > UINT32_MAX) {
				result = orng_rng_common_util_next64bynext32(c);
			}
			else {
				result = c->next32(c);
			}
#else
			result = c->next32(c);
#endif
		}
	}

	return (zend_long)((result % umax) + min);
}

static zend_object *create_object(zend_class_entry *ce)
{
	ORNG_MT19937_obj *obj = (ORNG_MT19937_obj*)ecalloc(1, sizeof(ORNG_MT19937_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, range, NULL, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_MT19937;
	obj->mode = ORNG_MT19937_MODE_NORMAL;
	return &obj->std;
}

static zend_object *create_object_php(zend_class_entry *ce)
{
	ORNG_MT19937_obj *obj = (ORNG_MT19937_obj*)ecalloc(1, sizeof(ORNG_MT19937_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, range_php, NULL, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_MT19937;
	obj->mode = ORNG_MT19937_MODE_PHP;
	return &obj->std;
}

static zend_object *create_object_mb(zend_class_entry *ce)
{
	ORNG_MT19937_obj *obj = (ORNG_MT19937_obj*)ecalloc(1, sizeof(ORNG_MT19937_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, range_mb, NULL, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_MT19937;
	obj->mode = ORNG_MT19937_MODE_MB;
	return &obj->std;
}

static void free_object(zend_object *object)
{
	ORNG_MT19937_obj *obj = ORNG_MT19937_obj_from_zend_object(object);
	zend_object_std_dtor(&obj->std);
	if (obj->common != NULL) {
		efree(obj->common);
	}
}

ORNG_COMPAT_RNG_CLONE_FUNCTION(MT19937)
{
	zend_object *old_obj = ORNG_COMPAT_RNG_CLONE_GET_OBJ();
	ORNG_MT19937_obj *old = ORNG_MT19937_obj_from_zend_object(old_obj);

	zend_object *new_obj;
	switch (old->mode) {
		case ORNG_MT19937_MODE_PHP:
			new_obj = create_object_php(old_obj->ce);
			break;
		case ORNG_MT19937_MODE_MB:
			new_obj = create_object_mb(old_obj->ce);
			break;
		default:
			new_obj = create_object(old_obj->ce);
			break;
	}
	ORNG_MT19937_obj *new = ORNG_MT19937_obj_from_zend_object(new_obj);

	zend_objects_clone_members(new_obj, old_obj);

	memcpy(new->state, old->state, sizeof(old->state));
	new->next = old->next;
	new->left = old->left;
	new->mode = old->mode;

	return new_obj;
}

/* {{{ \ORNG\MT19937::__construct(int $seed) */
PHP_METHOD(ORNG_MT19937, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());
	internal_mt_initialize(seed, obj->state);
}
/* }}} */

/* {{{ \ORNG\MT19937::next(): int */
PHP_METHOD(ORNG_MT19937, next)
{
	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	RETURN_LONG(obj->common->next32(obj->common) >> 1);
}
/* }}} */

/* {{{ \ORNG\ORNG_MT19937::next64(): int */
PHP_METHOD(ORNG_MT19937, next64)
{
	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());
	RETURN_LONG(orng_rng_common_util_next64bynext32(obj->common) >> 1);
}
/* }}} */

/* {{{ \ORNG\MT19937::range(int $min, int $max): int */
PHP_METHOD(ORNG_MT19937, range)
{
	zend_long min, max;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_RANGE();
	}

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	RETURN_LONG(obj->common->range(obj->common, min, max));
}
/* }}} */

/* {{{ \ORNG\MT19937::shuffle(array &$array): bool */
PHP_METHOD(ORNG_MT19937, shuffle)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	orng_rng_common_util_array_data_shuffle(obj->common, array);
	
	RETURN_TRUE;
}
/* }}} */

/* {{{ \ORNG\MT19937::arrayRand(array $array, int $num = 1): int|string|array */
// FIXME: duplicate code
PHP_METHOD(ORNG_MT19937, arrayRand)
{
	zval *input;
	zend_long num_req = 1;
	zend_string *string_key;
	zend_ulong num_key;
	int i;
	int num_avail;
	zend_bitset bitset;
	int negative_bitset = 0;
	uint32_t bitset_len;
	ALLOCA_FLAG(use_heap)

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(input)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(num_req)
	ZEND_PARSE_PARAMETERS_END();

	num_avail = zend_hash_num_elements(Z_ARRVAL_P(input));

	if (num_avail == 0) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_ARRAY_RAND_EMPTY();
	}

	if (num_req == 1) {
		HashTable *ht = Z_ARRVAL_P(input);

		if ((uint32_t)num_avail < ht->nNumUsed - (ht->nNumUsed>>1)) {
			/* If less than 1/2 of elements are used, don't sample. Instead search for a
			 * specific offset using linear scan. */
			zend_long i = 0;
			zend_long randval = obj->common->range(obj->common, 0, num_avail - 1);
			ZEND_HASH_FOREACH_KEY(Z_ARRVAL_P(input), num_key, string_key) {
				if (i == randval) {
					if (string_key) {
						RETURN_STR_COPY(string_key);
					} else {
						RETURN_LONG(num_key);
					}
				}
				i++;
			} ZEND_HASH_FOREACH_END();
		}

		/* Sample random buckets until we hit one that is not empty.
		 * The worst case probability of hitting an empty element is 1-1/2. The worst case
		 * probability of hitting N empty elements in a row is (1-1/2)**N.
		 * For N=10 this becomes smaller than 0.1%. */
		do {
			zend_long randval = obj->common->range(obj->common, 0, ht->nNumUsed - 1);
			Bucket *bucket = &ht->arData[randval];
			if (!Z_ISUNDEF(bucket->val)) {
				if (bucket->key) {
					RETURN_STR_COPY(bucket->key);
				} else {
					RETURN_LONG(bucket->h);
				}
			}
		} while (1);
	}

	if (num_req <= 0 || num_req > num_avail) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_ARRAY_RAND_AVAIL();
	}

	/* Make the return value an array only if we need to pass back more than one result. */
	array_init_size(return_value, (uint32_t)num_req);
	if (num_req > (num_avail >> 1)) {
		negative_bitset = 1;
		num_req = num_avail - num_req;
	}

	bitset_len = zend_bitset_len(num_avail);
	bitset = ZEND_BITSET_ALLOCA(bitset_len, use_heap);
	zend_bitset_clear(bitset, bitset_len);

	i = num_req;
	while (i) {
		zend_long randval = obj->common->range(obj->common, 0, num_avail - 1);
		if (!zend_bitset_in(bitset, randval)) {
			zend_bitset_incl(bitset, randval);
			i--;
		}
	}
	/* i = 0; */

	zend_hash_real_init_packed(Z_ARRVAL_P(return_value));
	ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(return_value)) {
		zval zv;
		/* We can't use zend_hash_index_find()
		 * because the array may have string keys or gaps. */
		ZEND_HASH_FOREACH_KEY(Z_ARRVAL_P(input), num_key, string_key) {
			if (zend_bitset_in(bitset, i) ^ negative_bitset) {
				if (string_key) {
					// ZEND_HASH_FILL_SET_STR_COPY(string_key);
					ZVAL_STR_COPY(&zv, string_key);
				} else {
					// ZEND_HASH_FILL_SET_LONG(num_key);
					ZVAL_LONG(&zv, num_key);
				}
				// ZEND_HASH_FILL_NEXT();
				ZEND_HASH_FILL_ADD(&zv);
			}
			i++;
		} ZEND_HASH_FOREACH_END();
	} ZEND_HASH_FILL_END();

	free_alloca(bitset, use_heap);
}
/* }}} */

/* {{{ \ORNG\ORNG_MT19937::strShuffle(string $string): string */
PHP_METHOD(ORNG_MT19937, strShuffle)
{
	zend_string *arg;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(arg)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));

	ORNG_MT19937_obj *obj = Z_ORNG_MT19937_P(getThis());

	if (Z_STRLEN_P(return_value) > 1) {
		orng_rng_common_util_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_mt19937)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(MT19937), class_ORNG_MT19937_methods);
	ce_ORNG_MT19937 = zend_register_internal_class(&ce);
	zend_class_implements(ce_ORNG_MT19937, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_MT19937->create_object = create_object;
	memcpy(&oh_MT19937, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_MT19937.offset = XtOffsetOf(ORNG_MT19937_obj, std);
	oh_MT19937.clone_obj = ORNG_COMPAT_RNG_CLONE(MT19937);
	oh_MT19937.free_obj = free_object;
	
	zend_class_entry ce2;
	INIT_CLASS_ENTRY(ce2, ORNG_RNG_FQN(MT19937PHP), class_ORNG_MT19937_methods);
	ce_ORNG_MT19937PHP = zend_register_internal_class(&ce2);
	zend_class_implements(ce_ORNG_MT19937PHP, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_MT19937PHP->create_object = create_object_php;

	zend_class_entry ce3;
	INIT_CLASS_ENTRY(ce3, ORNG_RNG_FQN(MT19937MB), class_ORNG_MT19937_methods);
	ce_ORNG_MT19937MB = zend_register_internal_class(&ce3);
	zend_class_implements(ce_ORNG_MT19937MB, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_MT19937MB->create_object = create_object_mb;

	return SUCCESS;
}
