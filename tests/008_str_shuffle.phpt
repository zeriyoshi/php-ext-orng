--TEST--
Check shuffle string.
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
    $string = 'bongobongobongobongo';
    $rng = new $class(SEED);
    $shuffled_string = $rng->strShuffle($string);
    for ($i = 0; $i < strlen($string); $i++) {
        if ($string[$i] !== $shuffled_string[$i]) {
            continue 2;
        }
    }
    die("NG, String not shuffled. class: ${class} string: ${shuffled_string}");
}
die('OK, All string shuffled.');

--EXPECT--
OK, All string shuffled.
