/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 60b32a3e32f2817da7c10b355a5748bebe0e0ced */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_XorShift128Plus___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_XorShift128Plus_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_XorShift128Plus_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_XorShift128Plus, __construct);
ZEND_METHOD(ORNG_XorShift128Plus, next);
ZEND_METHOD(ORNG_XorShift128Plus, range);


static const zend_function_entry class_ORNG_XorShift128Plus_methods[] = {
	ZEND_ME(ORNG_XorShift128Plus, __construct, arginfo_class_ORNG_XorShift128Plus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, next, arginfo_class_ORNG_XorShift128Plus_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, range, arginfo_class_ORNG_XorShift128Plus_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
