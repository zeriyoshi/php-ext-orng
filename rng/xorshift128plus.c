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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_bitset.h"

#include "../orng_util.h"
#include "../orng_compat.h"
#include "../php_orng.h"

#include "rnginterface.h"

#include "xorshift128plus.h"
#if PHP_VERSION_ID >= 80000
# include "xorshift128plus_arginfo.h"
#else
# include "xorshift128plus_arginfo_7.h"
#endif

PHPAPI zend_class_entry *ce_ORNG_XorShift128Plus;

static zend_object_handlers oh_XorShift128Plus;

static uint64_t internal_splitmix64_next(uint64_t *seed)
{
	uint64_t r;
	r = (*seed += 0x9e3779b97f4a7c15);
	r = (r ^ (r >> 30)) * 0xbf58476d1ce4e5b9;
	r = (r ^ (r >> 27)) * 0x94d049bb133111eb;
	return (r ^ (r >> 31));
}

static uint32_t next32(orng_rng_common *c)
{
	return (uint32_t) (c->next64(c) << 1);
}

static uint64_t next64(orng_rng_common *c)
{
	uint64_t s0, s1, r;

	ORNG_XorShift128Plus_obj *obj = ((ORNG_XorShift128Plus_obj*)c->obj);

	s1 = obj->s[0];
	s0 = obj->s[1];
	r = s0 + s1;
	obj->s[0] = s0;
	s1 ^= s1 << 23;
	obj->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

	return r;
}

static zend_long range(orng_rng_common *c, zend_long min, zend_long max)
{
	return orng_rng_common_util_rand_range(c, min, max);
}

static zend_object *create_object(zend_class_entry *ce)
{
	ORNG_XorShift128Plus_obj *obj = (ORNG_XorShift128Plus_obj*)ecalloc(1, sizeof(ORNG_XorShift128Plus_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, range, next64, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_XorShift128Plus;
	return &obj->std;
}

static void free_object(zend_object *object)
{
	ORNG_XorShift128Plus_obj *obj = ORNG_XorShift128Plus_obj_from_zend_object(object);
	zend_object_std_dtor(&obj->std);
	if (obj->common != NULL) {
		efree(obj->common);
	}
}

ORNG_COMPAT_RNG_CLONE_FUNCTION(XorShift128Plus)
{
	zend_object *old_obj = ORNG_COMPAT_RNG_CLONE_GET_OBJ();
	zend_object *new_obj = create_object(old_obj->ce);

	zend_objects_clone_members(new_obj, old_obj);

	ORNG_XorShift128Plus_obj *old = ORNG_XorShift128Plus_obj_from_zend_object(old_obj);
	ORNG_XorShift128Plus_obj *new = ORNG_XorShift128Plus_obj_from_zend_object(new_obj);

	memcpy(new->s, old->s, sizeof(old->s));

	return new_obj;
}

/* {{{ \ORNG\XorShift128Plus::__construct(int $seed) */
PHP_METHOD(ORNG_XorShift128Plus, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	obj->s[0] = internal_splitmix64_next(&seed);
	obj->s[1] = internal_splitmix64_next(&seed);
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::next(): int */
PHP_METHOD(ORNG_XorShift128Plus, next)
{
	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());
	RETURN_LONG(obj->common->next64(obj->common) >> 1);
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::range(int $min, int $max): int */
PHP_METHOD(ORNG_XorShift128Plus, range)
{
	zend_long min, max;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_RANGE();
	}

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	RETURN_LONG(obj->common->range(obj->common, min, max));
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::shuffle(array &$array): bool */
PHP_METHOD(ORNG_XorShift128Plus, shuffle)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	orng_rng_common_util_array_data_shuffle(obj->common, array);

	RETURN_TRUE;
}
/* }}} */

/* {{{ \ORNG\XorShift128Plus::arrayRand(array $array, int $num = 1): int|string|array */
// FIXME: duplicate code
PHP_METHOD(ORNG_XorShift128Plus, arrayRand)
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

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

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
			zend_long i = 0, randval = obj->common->range(obj->common, 0, num_avail - 1);
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

/* {{{ \ORNG\XorShift128Plus::strShuffle(string $string): string */
PHP_METHOD(ORNG_XorShift128Plus, strShuffle)
{
	zend_string *arg;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(arg)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));

	ORNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	if (Z_STRLEN_P(return_value) > 1) {
		orng_rng_common_util_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_xorshift128plus)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(XorShift128Plus), class_ORNG_XorShift128Plus_methods);
	ce_ORNG_XorShift128Plus = zend_register_internal_class(&ce);
	zend_class_implements(ce_ORNG_XorShift128Plus, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_XorShift128Plus->create_object = create_object;
	memcpy(&oh_XorShift128Plus, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_XorShift128Plus.offset = XtOffsetOf(ORNG_XorShift128Plus_obj, std);
	oh_XorShift128Plus.clone_obj = ORNG_COMPAT_RNG_CLONE(XorShift128Plus);
	oh_XorShift128Plus.free_obj = free_object;

	return SUCCESS;
}
