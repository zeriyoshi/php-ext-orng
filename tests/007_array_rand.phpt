--TEST--
Check array rand.
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

$array = ['a' => 0, 'b' => 1, 'c' => 2, 'd' => 3, 'e' => 4, 'f' => 5];
// one key
foreach ($classes as $class) {
    $rng = new $class(SEED);
    foreach ($array as $key => $value) {
        if ($rng->arrayRand($array, 1) !== $key) {
            continue 2;
        }
    }
    die("NG, Failed get randomize key. class: ${class}");
}
// more keys.
foreach ($classes as $class) {
    $rng = new $class(SEED);
    [$key1, $key2] = $rng->arrayRand($array, 2);
    if (
        ! array_key_exists($key1, $array) ||
        ! array_key_exists($key2, $array) ||
        ($key1 === 'a' && $key2 === 'b')
    ) {
        die("NG, Failed get multiple random keys. class: ${class}");
    }
}
die('OK, All works correctly.');
--EXPECT--
OK, All works correctly.
