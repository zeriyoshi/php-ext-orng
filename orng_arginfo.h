/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7e3885031c98b27dfc0f365d51f5da5311f1ccf6 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_RNGInterface___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_ORNG_XorShift128Plus___construct arginfo_class_ORNG_RNGInterface___construct

#define arginfo_class_ORNG_XorShift128Plus_next arginfo_class_ORNG_RNGInterface_next

#define arginfo_class_ORNG_XorShift128Plus_range arginfo_class_ORNG_RNGInterface_range


ZEND_METHOD(ORNG_XorShift128Plus, __construct);
ZEND_METHOD(ORNG_XorShift128Plus, next);
ZEND_METHOD(ORNG_XorShift128Plus, range);


static const zend_function_entry class_ORNG_RNGInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, __construct, arginfo_class_ORNG_RNGInterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, next, arginfo_class_ORNG_RNGInterface_next, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, range, arginfo_class_ORNG_RNGInterface_range, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};


static const zend_function_entry class_ORNG_XorShift128Plus_methods[] = {
	ZEND_ME(ORNG_XorShift128Plus, __construct, arginfo_class_ORNG_XorShift128Plus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, next, arginfo_class_ORNG_XorShift128Plus_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, range, arginfo_class_ORNG_XorShift128Plus_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
