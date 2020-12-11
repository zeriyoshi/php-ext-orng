--TEST--
Check shuffle array.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php

const SEED = 1000;

$classes = include('data/classes.inc');

foreach ($classes as $class) {
    $array = range(0, 999);
    $rng = new $class(SEED);
    $rng->shuffle($array);
    foreach (range(0, 999) as $i) {
        foreach ($array as $key => $value) {
            if ($key !== $i) {
                break 2;
            }
        }
        die("NG, Array not shuffled. class: ${class}");
    }
}
die('OK, All array shuffled.');

--EXPECT--
OK, All array shuffled.
