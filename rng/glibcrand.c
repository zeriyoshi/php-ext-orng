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
#if PHP_VERSION_ID >= 80000
# include "glibcrand_arginfo.h"
#else
# include "glibcrand_arginfo_7.h"
#endif

PHPAPI zend_class_entry *ce_ORNG_GLibCRand;

static zend_object_handlers oh_GLibCRand;

// from upstream: https://github.com/php/php-src/blob/PHP-7.0/ext/standard/array.c#L2249
static void internal_array_data_shuffle(orng_rng_common *c, zval *array)
{
	uint32_t idx, j, n_elems;
	Bucket *p, temp;
	HashTable *hash;
	zend_long rnd_idx;
	uint32_t n_left;

	n_elems = zend_hash_num_elements(Z_ARRVAL_P(array));

	if (n_elems < 1) {
		return;
	}

	hash = Z_ARRVAL_P(array);
	n_left = n_elems;

	if (EXPECTED(hash->u.v.nIteratorsCount == 0)) {
		if (hash->nNumUsed != hash->nNumOfElements) {
			for (j = 0, idx = 0; idx < hash->nNumUsed; idx++) {
				p = hash->arData + idx;
				if (Z_TYPE(p->val) == IS_UNDEF) continue;
				if (j != idx) {
					hash->arData[j] = *p;
				}
				j++;
			}
		}
		while (--n_left) {
			rnd_idx = c->next32(c);
			ORNG_GLIBCRAND_RAND_RANGE(rnd_idx, 0, n_left, ORNG_GLIBCRAND_RAND_MAX);
			if (rnd_idx != n_left) {
				temp = hash->arData[n_left];
				hash->arData[n_left] = hash->arData[rnd_idx];
				hash->arData[rnd_idx] = temp;
			}
		}
	} else {
		uint32_t iter_pos = zend_hash_iterators_lower_pos(hash, 0);

		if (hash->nNumUsed != hash->nNumOfElements) {
			for (j = 0, idx = 0; idx < hash->nNumUsed; idx++) {
				p = hash->arData + idx;
				if (Z_TYPE(p->val) == IS_UNDEF) continue;
				if (j != idx) {
					hash->arData[j] = *p;
					if (idx == iter_pos) {
						zend_hash_iterators_update(hash, idx, j);
						iter_pos = zend_hash_iterators_lower_pos(hash, iter_pos + 1);
					}
				}
				j++;
			}
		}
		while (--n_left) {
			rnd_idx = c->next32(c);
			ORNG_GLIBCRAND_RAND_RANGE(rnd_idx, 0, n_left, ORNG_GLIBCRAND_RAND_MAX);
			if (rnd_idx != n_left) {
				temp = hash->arData[n_left];
				hash->arData[n_left] = hash->arData[rnd_idx];
				hash->arData[rnd_idx] = temp;
				zend_hash_iterators_update(hash, (uint32_t)rnd_idx, n_left);
			}
		}
	}
	HANDLE_BLOCK_INTERRUPTIONS();
	hash->nNumUsed = n_elems;
	hash->nInternalPointer = 0;

	for (j = 0; j < n_elems; j++) {
		p = hash->arData + j;
		if (p->key) {
			zend_string_release(p->key);
		}
		p->h = j;
		p->key = NULL;
	}
	hash->nNextFreeElement = n_elems;
	if (!(hash->u.flags & HASH_FLAG_PACKED)) {
		zend_hash_to_packed(hash);
	}
	HANDLE_UNBLOCK_INTERRUPTIONS();
}

// from upstream: https://github.com/php/php-src/blob/PHP-7.0/ext/standard/string.c#L5353
static void internal_string_shuffle(orng_rng_common *c, char *str, zend_long len) /* {{{ */
{
	zend_long n_elems, rnd_idx, n_left;
	char temp;
	/* The implementation is stolen from array_data_shuffle       */
	/* Thus the characteristics of the randomization are the same */
	n_elems = len;

	if (n_elems <= 1) {
		return;
	}

	n_left = n_elems;

	while (--n_left) {
		rnd_idx = c->next32(c);
		ORNG_GLIBCRAND_RAND_RANGE(rnd_idx, 0, n_left, ORNG_GLIBCRAND_RAND_MAX);
		if (rnd_idx != n_left) {
			temp = str[n_left];
			str[n_left] = str[rnd_idx];
			str[rnd_idx] = temp;
		}
	}
}

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

	internal_array_data_shuffle(obj->common, array);

	RETURN_TRUE;
}
/* }}} */

/* {{{ \ORNG\GLibCRand::arrayRand(array $array, int $num = 1): int|string|array */
// FIXME: duplicate code
PHP_METHOD(ORNG_GLibCRand, arrayRand)
{
	zval *input;
	zend_long randval, num_req = 1;
	int num_avail;
	zend_string *string_key;
	zend_ulong num_key;

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

	if (ZEND_NUM_ARGS() > 1) {
		if (num_req <= 0 || num_req > num_avail) {
			ORNG_COMPAT_RETURN_ERROR_OR_THROW_ARRAY_RAND_AVAIL();
		}
	}

	/* Make the return value an array only if we need to pass back more than one result. */
	if (num_req > 1) {
		array_init_size(return_value, (uint32_t)num_req);
	}

	/* We can't use zend_hash_index_find() because the array may have string keys or gaps. */
	ZEND_HASH_FOREACH_KEY(Z_ARRVAL_P(input), num_key, string_key) {
		if (!num_req) {
			break;
		}

		randval = obj->common->next32(obj->common);

		if ((double) (randval / (ORNG_GLIBCRAND_RAND_MAX + 1.0)) < (double) num_req / (double) num_avail) {
			/* If we are returning a single result, just do it. */
			if (Z_TYPE_P(return_value) != IS_ARRAY) {
				if (string_key) {
					RETURN_STR_COPY(string_key);
				} else {
					RETURN_LONG(num_key);
				}
			} else {
				/* Append the result to the return value. */
				if (string_key) {
					add_next_index_str(return_value, zend_string_copy(string_key));
				} else {
					add_next_index_long(return_value, num_key);
				}
			}
			num_req--;
		}
		num_avail--;
	} ZEND_HASH_FOREACH_END();
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
		internal_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_glibcrand)
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
