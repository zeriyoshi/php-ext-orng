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
#include "standard/php_var.h"

#include "zend_bitset.h"
#include "zend_interfaces.h"
#include "zend_smart_str.h"
#include "zend_exceptions.h"

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

static uint32_t next32(orng_rng_common *c)
{
	unsigned int r;

	ORNG_GLibCRand_obj *obj = ((ORNG_GLibCRand_obj*)c->obj);

	obj->r[obj->next % ORNG_GLIBCRAND_R] = obj->r[(obj->next + 313) % ORNG_GLIBCRAND_R] + obj->r[(obj->next + 341) % ORNG_GLIBCRAND_R];
	r = ((unsigned int) obj->r[obj->next % ORNG_GLIBCRAND_R] >> 1);
	obj->next = (obj->next + 1) % ORNG_GLIBCRAND_R;
	return r;
}

static zend_long range(orng_rng_common *c, zend_long min, zend_long max)
{
	uint32_t n = c->next32(c);
	ORNG_GLIBCRAND_RAND_RANGE(n, min, max, ORNG_GLIBCRAND_RAND_MAX);
	return n;
}

static zend_object *create_object(zend_class_entry *ce)
{
	ORNG_GLibCRand_obj *obj = (ORNG_GLibCRand_obj*)ecalloc(1, sizeof(ORNG_GLibCRand_obj) + zend_object_properties_size(ce));
	orng_rng_common *c = orng_rng_common_initialize(next32, range, NULL, obj);
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

	unsigned int useed = (unsigned int) seed;

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
	for (i = 31; i < 34; i++) {
		obj->r[i] = obj->r[i - 31];
	}
	for (i = 34; i < ORNG_GLIBCRAND_R; i++) {
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

/* {{{ \ORNG\GLibCRand::next64(): int */
PHP_METHOD(ORNG_GLibCRand, next64)
{
	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());
	RETURN_LONG(orng_rng_common_util_next64bynext32(obj->common) >> 1);
}
/* }}} */

/* {{{ \ORNG\GLibCRand::range(int $min, int $max): int */
PHP_METHOD(ORNG_GLibCRand, range)
{
	zend_long min, max;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		ORNG_COMPAT_RETURN_ERROR_OR_THROW_RANGE();
	}

	ORNG_GLibCRand_obj *obj = Z_ORNG_GLibCRand_P(getThis());

	RETURN_LONG(obj->common->range(obj->common, min, max));
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
		orng_rng_common_util_string_shuffle(obj->common, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

/* {{{ \ORNG\GLibCRand::serialize(): string */
PHP_METHOD(ORNG_GLibCRand, serialize)
{
	ORNG_GLibCRand_obj *intern;
	php_serialize_data_t var_hash;
	smart_str buf = {0};
	int i;

	if (zend_parse_parameters_none() == FAILURE) {
		ORNG_COMPAT_RETURN_ERROR_OR_NOTHING_SERIALIZE();
	}

	intern = Z_ORNG_GLibCRand_P(getThis());
	
	PHP_VAR_SERIALIZE_INIT(var_hash);

	/* state */
	zval r, next;
	smart_str_appendl(&buf, "r:", 2);
	ZVAL_LONG(&next, intern->next);
	php_var_serialize(&buf, &next, &var_hash);
	for (i = 0; i < ORNG_GLIBCRAND_R; i++) {
		ZVAL_LONG(&r, intern->r[i]);
		php_var_serialize(&buf, &r, &var_hash);
	}

	/* members */
	zval members;
	smart_str_appendl(&buf, "m:", 2);
	ZVAL_ARR(&members, zend_array_dup(zend_std_get_properties(ORNG_COMPAT_ZVAL_GETTHIS())));
	php_var_serialize(&buf, &members, &var_hash);
	zval_ptr_dtor(&members);

	PHP_VAR_SERIALIZE_DESTROY(var_hash);
	
	RETURN_NEW_STR(buf.s);
}
/* }}} */

/* {{{ \ORNG\GLibCRand::unserialize(string $serialized): void */
PHP_METHOD(ORNG_GLibCRand, unserialize)
{
	ORNG_GLibCRand_obj *intern;
	char *buf;
	size_t buf_len;
	const unsigned char *p, *s;
	php_unserialize_data_t var_hash;
	zval *pcount, *pmembers;
	zend_long count;
	int i;

	intern = Z_ORNG_GLibCRand_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &buf, &buf_len) == FAILURE) {
		ORNG_COMPAT_RETURN_ERROR_OR_NOTHING_SERIALIZE();
	}

	if (buf_len == 0) {
		return;
	}

	/* state */
	s = p = (const unsigned char*) buf;
	PHP_VAR_UNSERIALIZE_INIT(var_hash);

	if (*p!= 'r' || *++p != ':') {
		goto outexcept;
	}
	p++;

	pcount = var_tmp_var(&var_hash);
	if (! php_var_unserialize(pcount, &p, s + buf_len, &var_hash) || Z_TYPE_P(pcount) != IS_LONG) {
		goto outexcept;
	}
	intern->next = Z_LVAL_P(pcount);
	for (i = 0; i < ORNG_GLIBCRAND_R; i++) {
		if (! php_var_unserialize(pcount, &p, s + buf_len, &var_hash) || Z_TYPE_P(pcount) != IS_LONG) {
			goto outexcept;
		}
		intern->r[i] = Z_LVAL_P(pcount);
	}

	/* members */
	if (*p!= 'm' || *++p != ':') {
		goto outexcept;
	}
	++p;

	pmembers = var_tmp_var(&var_hash);
	if (! php_var_unserialize(pmembers, &p, s + buf_len, &var_hash) || Z_TYPE_P(pmembers) != IS_ARRAY) {
		goto outexcept;
	}
	object_properties_load(&intern->std, Z_ARRVAL_P(pmembers));

	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	return;

outexcept:
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
	zend_throw_exception_ex(zend_ce_error_exception, 0, "Error at offset %zd of %zd bytes", ((char*)p - buf), buf_len);
	return;
}
/* }}} */

PHP_MINIT_FUNCTION(orng_rng_glibcrand)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, ORNG_RNG_FQN(GLibCRand), class_ORNG_GLibCRand_methods);
	ce_ORNG_GLibCRand = zend_register_internal_class(&ce);
	zend_class_implements(ce_ORNG_GLibCRand, 1, orng_ce_ORNG_RNGInterface);
	zend_class_implements(ce_ORNG_GLibCRand, 1, zend_ce_serializable);
	ce_ORNG_GLibCRand->create_object = create_object;
	memcpy(&oh_GLibCRand, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	oh_GLibCRand.offset = XtOffsetOf(ORNG_GLibCRand_obj, std);
	oh_GLibCRand.clone_obj = ORNG_COMPAT_RNG_CLONE(GLibCRand);
	oh_GLibCRand.free_obj = free_object;

	return SUCCESS;
}
