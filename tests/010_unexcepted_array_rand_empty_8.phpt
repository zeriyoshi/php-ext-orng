--TEST--
Check arrayRand method in unexcepted empty array. (PHP >= 8.0)
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
        (new $class(SEED))->arrayRand([], 1);
    } catch (ValueError $e) {
        echo $e . PHP_EOL;
    }
}
?>
--EXPECTF--
ValueError: ORNG\GLibCRand::arrayRand(): Argument #1 ($array) cannot be empty in %s:%d
Stack trace:
#0 %s(%d): ORNG\GLibCRand->arrayRand(Array, 1)
#1 {main}
ValueError: ORNG\XorShift128Plus::arrayRand(): Argument #1 ($array) cannot be empty in %s:%d
Stack trace:
#0 %s(%d): ORNG\XorShift128Plus->arrayRand(Array, 1)
#1 {main}
ValueError: ORNG\MT1%d%d37::arrayRand(): Argument #1 ($array) cannot be empty in %s:%d
Stack trace:
#0 %s(%d): ORNG\MT1%d%d37->arrayRand(Array, 1)
#1 {main}
ValueError: ORNG\MT1%d%d37PHP::arrayRand(): Argument #1 ($array) cannot be empty in %s:%d
Stack trace:
#0 %s(%d): ORNG\MT1%d%d37PHP->arrayRand(Array, 1)
#1 {main}