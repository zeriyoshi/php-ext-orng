/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 409fb5420e5c1d6f9b5749decd7e4536e76212cf */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_MT19937PHP___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937PHP_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937PHP_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937PHP_shuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_ORNG_MT19937PHP_arrayRand, 0, 1, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_ARRAY)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, num, IS_LONG, 0, "1")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937PHP_strShuffle, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_MT19937PHP, __construct);
ZEND_METHOD(ORNG_MT19937PHP, next);
ZEND_METHOD(ORNG_MT19937PHP, range);
ZEND_METHOD(ORNG_MT19937PHP, shuffle);
ZEND_METHOD(ORNG_MT19937PHP, arrayRand);
ZEND_METHOD(ORNG_MT19937PHP, strShuffle);


static const zend_function_entry class_ORNG_MT19937PHP_methods[] = {
	ZEND_ME(ORNG_MT19937PHP, __construct, arginfo_class_ORNG_MT19937PHP___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, next, arginfo_class_ORNG_MT19937PHP_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, range, arginfo_class_ORNG_MT19937PHP_range, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, shuffle, arginfo_class_ORNG_MT19937PHP_shuffle, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, arrayRand, arginfo_class_ORNG_MT19937PHP_arrayRand, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, strShuffle, arginfo_class_ORNG_MT19937PHP_strShuffle, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
