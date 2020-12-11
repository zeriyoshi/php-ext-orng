/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ffb8e5579812a395aeffb0c3a904e921a7f9ca4b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_RNGInterface___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_shuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_ORNG_RNGInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, __construct, arginfo_class_ORNG_RNGInterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, next, arginfo_class_ORNG_RNGInterface_next, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, range, arginfo_class_ORNG_RNGInterface_range, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, shuffle, arginfo_class_ORNG_RNGInterface_shuffle, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};
