/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 865aee480a9b8b296612250899aab17d61464128 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_RNGInterface___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_RNGInterface_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_ORNG_GLibCRand___construct arginfo_class_ORNG_RNGInterface___construct

#define arginfo_class_ORNG_GLibCRand_next arginfo_class_ORNG_RNGInterface_next

#define arginfo_class_ORNG_GLibCRand_range arginfo_class_ORNG_RNGInterface_range

#define arginfo_class_ORNG_XorShift128Plus___construct arginfo_class_ORNG_RNGInterface___construct

#define arginfo_class_ORNG_XorShift128Plus_next arginfo_class_ORNG_RNGInterface_next

#define arginfo_class_ORNG_XorShift128Plus_range arginfo_class_ORNG_RNGInterface_range

#define arginfo_class_ORNG_MT19937___construct arginfo_class_ORNG_RNGInterface___construct

#define arginfo_class_ORNG_MT19937_next arginfo_class_ORNG_RNGInterface_next

#define arginfo_class_ORNG_MT19937_range arginfo_class_ORNG_RNGInterface_range

#define arginfo_class_ORNG_MT19937PHP___construct arginfo_class_ORNG_RNGInterface___construct

#define arginfo_class_ORNG_MT19937PHP_next arginfo_class_ORNG_RNGInterface_next

#define arginfo_class_ORNG_MT19937PHP_range arginfo_class_ORNG_RNGInterface_range


ZEND_METHOD(ORNG_GLibCRand, __construct);
ZEND_METHOD(ORNG_GLibCRand, next);
ZEND_METHOD(ORNG_GLibCRand, range);
ZEND_METHOD(ORNG_XorShift128Plus, __construct);
ZEND_METHOD(ORNG_XorShift128Plus, next);
ZEND_METHOD(ORNG_XorShift128Plus, range);
ZEND_METHOD(ORNG_MT19937, __construct);
ZEND_METHOD(ORNG_MT19937, next);
ZEND_METHOD(ORNG_MT19937, range);
ZEND_METHOD(ORNG_MT19937PHP, __construct);
ZEND_METHOD(ORNG_MT19937PHP, next);
ZEND_METHOD(ORNG_MT19937PHP, range);


static const zend_function_entry class_ORNG_RNGInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, __construct, arginfo_class_ORNG_RNGInterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, next, arginfo_class_ORNG_RNGInterface_next, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(ORNG_RNGInterface, range, arginfo_class_ORNG_RNGInterface_range, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};


static const zend_function_entry class_ORNG_GLibCRand_methods[] = {
	ZEND_ME(ORNG_GLibCRand, __construct, arginfo_class_ORNG_GLibCRand___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_GLibCRand, next, arginfo_class_ORNG_GLibCRand_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_GLibCRand, range, arginfo_class_ORNG_GLibCRand_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_ORNG_XorShift128Plus_methods[] = {
	ZEND_ME(ORNG_XorShift128Plus, __construct, arginfo_class_ORNG_XorShift128Plus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, next, arginfo_class_ORNG_XorShift128Plus_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_XorShift128Plus, range, arginfo_class_ORNG_XorShift128Plus_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_ORNG_MT19937_methods[] = {
	ZEND_ME(ORNG_MT19937, __construct, arginfo_class_ORNG_MT19937___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, next, arginfo_class_ORNG_MT19937_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, range, arginfo_class_ORNG_MT19937_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_ORNG_MT19937PHP_methods[] = {
	ZEND_ME(ORNG_MT19937PHP, __construct, arginfo_class_ORNG_MT19937PHP___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, next, arginfo_class_ORNG_MT19937PHP_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, range, arginfo_class_ORNG_MT19937PHP_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
