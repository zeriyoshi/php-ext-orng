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

#include "../orng_compat.h"
#include "../php_orng.h"

#include "rnginterface.h"

#include "glibcrand.h"
#include "glibcrand_arginfo.h"

PHPAPI zend_class_entry *ce_ORNG_GLibCRand;

static zend_object_handlers oh_GLibCRand;

static uint32_t next32(orng_rng_common *c)
{
	unsigned int r;

	ORNG_GLibCRand_obj *obj = ((ORNG_GLibCRand_obj*)c->obj);

	obj->r[obj->next % 344] = obj->r[(obj->next + 313) % 344] + obj->r[(obj->next + 341) % 344];
	r = ((unsigned int) obj->r[obj->next % 344] >> 1);
	obj->next = (obj->next + 1) % 344;
	return r;
}

static zend_object *create_object(zend_class_entry *ce)
{
	ORNG_GLibCRand_obj *obj = (ORNG_GLibCRand_obj*)ecalloc(1, sizeof(ORNG_GLibCRand_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, NULL, obj);
	obj->common = c;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &oh_GLibCRand;
	return &obj->std;
}

static void free_object(zend_object *object)
{
	ORNG_GLibCRand_obj *obj = ORNG_GLibCRand_obj_from_zend_object(object);
	zend_object_std_dtor(&obj->std);
	if (obj->common != NULL) {
		efree(obj->common);
	}
}

ORNG_COMPAT_RNG_CLONE_FUNCTION(GLibCRand)
{
	zend_object *old_obj = ORNG_COMPAT_RNG_CLONE_GET_OBJ();
	zend_object *new_obj = create_object(old_obj->ce);

	zend_objects_clone_members(new_obj, old_obj);

	ORNG_GLibCRand_obj *old = ORNG_GLibCRand_obj_from_zend_object(old_obj);
	ORNG_GLibCRand_obj *new = ORNG_GLibCRand_obj_from_zend_object(new_obj);

	memcpy(new->r, old->r, sizeof(old->r));
	new->next = old->next;

	return new_obj;
}

/* {{{ \ORNG\GLibCRand::__construct(int $seed) */
PHP_METHOD(ORNG_GLibCRand, __construct)
{
	zend_long seed;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	long useed = (unsigned int) seed;

	if (useed == 0) {
		useed = 1;
	}

	obj->r[0] = useed;
	for (i = 1; i< 31; i++) {
		obj->r[i] = (16807LL * obj->r[i - 1]) % 2147483647;
		if (obj->r[i] < 0) {
			obj->r[i] += 2147483647;
		}
	}
	for (i = 31; i< 34; i++) {
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
	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());
	RETURN_LONG(obj->common->next32(obj->common));
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

	if (UNEXPECTED(max < min)) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_RANGE();
	}

	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	n = obj->common->next32(obj->common);
	n = min + (zend_long) ((double) ((double) max - min + 1.0) * (n / (2147483647 + 1.0)));

	RETURN_LONG(n);
}
/* }}} */

/* {{{ \ORNG\GLibCRand::shuffle(array &$array): bool */
PHP_METHOD(ORNG_GLibCRand, shuffle)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	orng_rng_common_util_array_data_shuffle(obj->common, array);

	RETURN_TRUE;
}
/* }}} */

/* {{{ \ORNG\GLibCRand::arrayRand(array $array, int $num = 1): int|string|array */
// FIXME: duplicate code
PHP_METHOD(ORNG_GLibCRand, arrayRand)
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

	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

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
			zend_long i = 0, randval = orng_rng_common_util_range(obj->common, 0, num_avail - 1);
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
			zend_long randval = orng_rng_common_util_range(obj->common, 0, ht->nNumUsed - 1);
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
		zend_long randval = orng_rng_common_util_range(obj->common, 0, num_avail - 1);
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

/* {{{ \ORNG\GLibCRand::strShuffle(string $string): string */
PHP_METHOD(ORNG_GLibCRand, strShuffle)
{
	zend_string *arg;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(arg)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));

	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	if (Z_STRLEN_P(return_value) > 1) {
		orng_rng_common_util_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

PHP_MINIT_FUNCTION(orng_glibcrand)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(GLibCRand), class_ORNG_GLibCRand_methods);
	ce_ORNG_GLibCRand = zend_register_internal_class(&ce);
	zend_class_implements(ce_ORNG_GLibCRand, 1, orng_ce_ORNG_RNGInterface);
	ce_ORNG_GLibCRand->create_object = create_object;
	memcpy(&oh_GLibCRand, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_GLibCRand.offset = XtOffsetOf(ORNG_GLibCRand_obj, std);
	oh_GLibCRand.clone_obj = ORNG_COMPAT_RNG_CLONE(GLibCRand);
	oh_GLibCRand.free_obj = free_object;

	return SUCCESS;
}
