/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7c8b7524f27ec2a7454e5b10b1fba461ead0812c */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_GLibCRand___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_GLibCRand_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_GLibCRand_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_GLibCRand_shuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_GLibCRand, __construct);
ZEND_METHOD(ORNG_GLibCRand, next);
ZEND_METHOD(ORNG_GLibCRand, range);
ZEND_METHOD(ORNG_GLibCRand, shuffle);


static const zend_function_entry class_ORNG_GLibCRand_methods[] = {
	ZEND_ME(ORNG_GLibCRand, __construct, arginfo_class_ORNG_GLibCRand___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_GLibCRand, next, arginfo_class_ORNG_GLibCRand_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_GLibCRand, range, arginfo_class_ORNG_GLibCRand_range, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_GLibCRand, shuffle, arginfo_class_ORNG_GLibCRand_shuffle, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
