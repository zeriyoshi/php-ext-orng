--TEST--
Check arrayRand method in unexcepted over num. (PHP >= 8.0)
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (version_compare(PHP_VERSION, '8.0.0', '<')) {
    die('skip: this test only available on PHP >= 8.0.');
}
?>
--FILE--
<?php

const SEED = 10;

$classes = include('data/classes.inc');

foreach ($classes as $class) {
    try {
        (new $class(SEED))->arrayRand(['a' => 1], 2);
    } catch (ValueError $e) {
        echo $e . PHP_EOL;
    }
}
?>
--EXPECTF--
ValueError: ORNG\GLibCRand::arrayRand(): Argument #2 ($num) must be between 1 and the number of elements in argument #1 ($array) in %s:%d
Stack trace:
#0 %s(%d): ORNG\GLibCRand->arrayRand(Array, 2)
#1 {main}
ValueError: ORNG\XorShift128Plus::arrayRand(): Argument #2 ($num) must be between 1 and the number of elements in argument #1 ($array) in %s:%d
Stack trace:
#0 %s(%d): ORNG\XorShift128Plus->arrayRand(Array, 2)
#1 {main}
ValueError: ORNG\MT19937::arrayRand(): Argument #2 ($num) must be between 1 and the number of elements in argument #1 ($array) in %s:%d
Stack trace:
#0 %s(%d): ORNG\MT19937->arrayRand(Array, 2)
#1 {main}
ValueError: ORNG\MT19937PHP::arrayRand(): Argument #2 ($num) must be between 1 and the number of elements in argument #1 ($array) in %s:%d
Stack trace:
#0 %s(%d): ORNG\MT19937PHP->arrayRand(Array, 2)
#1 {main}