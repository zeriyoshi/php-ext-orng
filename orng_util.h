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

#ifndef ORNG_UTIL_H
# define ORNG_UTIL_H

# include "php.h"

typedef struct _orng_rng_common orng_rng_common;

typedef struct _orng_rng_common {
	uint32_t (*next32)(orng_rng_common*);
	zend_long (*range)(orng_rng_common*, zend_long, zend_long);
	uint64_t (*next64)(orng_rng_common*);
	void* obj;
} orng_rng_common;

static orng_rng_common* orng_rng_common_initialize(
	uint32_t (*next32)(orng_rng_common*),
	zend_long (*range)(orng_rng_common*, zend_long, zend_long),
	uint64_t (*next64)(orng_rng_common*),
	void *obj
) {
	orng_rng_common *c = (orng_rng_common*)ecalloc(1, sizeof(orng_rng_common));
	c->next32 = next32;
	c->range = range;
	c->next64 = next64;
	c->obj = obj;

	return c;
}

static uint64_t orng_rng_common_util_next64bynext32(orng_rng_common *common)
{
	uint64_t r;

	r = common->next32(common);
	r = (r << 32) | common->next32(common);

	return r;
}

/* from upstream: https://github.com/php/php-src/blob/2b5de6f839feea0ae1d5289d59dd7f159fcdcc8c/ext/standard/mt_rand.c#L237 */
# if ZEND_ULONG_MAX > UINT32_MAX
static uint64_t orng_rng_common_util_rand_range64(orng_rng_common *c, uint64_t umax)
{
	uint64_t result, limit;

	if (c->next64 != NULL) {
		result = c->next64(c);
	} else {
		result = orng_rng_common_util_next64bynext32(c);
	}
	

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
		if (c->next64 != NULL) {
			result = c->next64(c);
		} else {
			result = orng_rng_common_util_next64bynext32(c);
		}
	}

	return result % umax;
}
# endif

/* from upstream: https://github.com/php/php-src/blob/2b5de6f839feea0ae1d5289d59dd7f159fcdcc8c/ext/standard/mt_rand.c#L207 */
static uint32_t orng_rng_common_util_rand_range32(orng_rng_common *c, uint32_t umax)
{
	uint32_t result, limit;

	result = c->next32(c);

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
		result = c->next32(c);
	}

	return result % umax;
}

/* from upstream: https://github.com/php/php-src/blob/2b5de6f839feea0ae1d5289d59dd7f159fcdcc8c/ext/standard/mt_rand.c#L270 */
static zend_long orng_rng_common_util_rand_range(orng_rng_common *c, zend_long min, zend_long max)
{
	zend_ulong umax = max - min;
# if ZEND_ULONG_MAX > UINT32_MAX
	if (umax > UINT32_MAX) {
		return (zend_long) (orng_rng_common_util_rand_range64(c, umax) + min);
	}
# endif
	return (zend_long) (orng_rng_common_util_rand_range32(c, umax) + min);
}

/* from upstream: https://github.com/php/php-src/blob/8591bb70a4b22a3bb7ca897bface89fcc2b85d64/ext/standard/array.c#L2880 */
static void orng_rng_common_util_array_data_shuffle(orng_rng_common *c, zval *array)
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

	if (EXPECTED(!HT_HAS_ITERATORS(hash))) {
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
			rnd_idx = c->range(c, 0, n_left);
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
			rnd_idx = c->range(c, 0, n_left);
			if (rnd_idx != n_left) {
				temp = hash->arData[n_left];
				hash->arData[n_left] = hash->arData[rnd_idx];
				hash->arData[rnd_idx] = temp;
				zend_hash_iterators_update(hash, (uint32_t)rnd_idx, n_left);
			}
		}
	}
	hash->nNumUsed = n_elems;
	hash->nInternalPointer = 0;

	for (j = 0; j < n_elems; j++) {
		p = hash->arData + j;
		if (p->key) {
			zend_string_release_ex(p->key, 0);
		}
		p->h = j;
		p->key = NULL;
	}
	hash->nNextFreeElement = n_elems;
	if (!(HT_FLAGS(hash) & HASH_FLAG_PACKED)) {
		zend_hash_to_packed(hash);
	}
}

/* from upstream: https://github.com/php/php-src/blob/9234446ca04a4bf897988d977835d03918148b12/ext/standard/string.c#L5659 */
static void orng_rng_common_util_string_shuffle(orng_rng_common *c, char *str, zend_long len)
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
		rnd_idx = c->range(c, 0, n_left);
		if (rnd_idx != n_left) {
			temp = str[n_left];
			str[n_left] = str[rnd_idx];
			str[rnd_idx] = temp;
		}
	}
}
#endif
