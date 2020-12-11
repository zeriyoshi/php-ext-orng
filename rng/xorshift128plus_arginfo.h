/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 55f3dac6dc325d8215b5ee70b35e760eda0a10d8 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_XorShift128Plus___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_XorShift128Plus_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_XorShift128Plus_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_XorShift128Plus_shuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_ORNG_XorShift128Plus_arrayRand, 0, 1, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_ARRAY)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, num, IS_LONG, 0, "1")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_XorShift128Plus_strShuffle, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_XorShift128Plus, __construct);
ZEND_METHOD(ORNG_XorShift128Plus, next);
ZEND_METHOD(ORNG_XorShift128Plus, range);
ZEND_METHOD(ORNG_XorShift128Plus, shuffle);
ZEND_METHOD(ORNG_XorShift128Plus, arrayRand);
ZEND_METHOD(ORNG_XorShift128Plus, strShuffle);


static const zend_function_entry class_ORNG_XorShift128Plus_methods[] = {
	ZEND_ME(ORNG_XorShift128Plus, __construct, arginfo_class_ORNG_XorShift128Plus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, next, arginfo_class_ORNG_XorShift128Plus_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, range, arginfo_class_ORNG_XorShift128Plus_range, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, shuffle, arginfo_class_ORNG_XorShift128Plus_shuffle, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, arrayRand, arginfo_class_ORNG_XorShift128Plus_arrayRand, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, strShuffle, arginfo_class_ORNG_XorShift128Plus_strShuffle, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
