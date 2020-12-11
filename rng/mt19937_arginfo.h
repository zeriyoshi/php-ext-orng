/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f50deb0faff5ca8b6aa2096f9b5ecdb9fae29aa9 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_MT19937___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937_shuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937_strShuffle, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_MT19937, __construct);
ZEND_METHOD(ORNG_MT19937, next);
ZEND_METHOD(ORNG_MT19937, range);
ZEND_METHOD(ORNG_MT19937, shuffle);
ZEND_METHOD(ORNG_MT19937, strShuffle);


static const zend_function_entry class_ORNG_MT19937_methods[] = {
	ZEND_ME(ORNG_MT19937, __construct, arginfo_class_ORNG_MT19937___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, next, arginfo_class_ORNG_MT19937_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, range, arginfo_class_ORNG_MT19937_range, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, shuffle, arginfo_class_ORNG_MT19937_shuffle, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, strShuffle, arginfo_class_ORNG_MT19937_strShuffle, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
