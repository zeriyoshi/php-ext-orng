--TEST--
Check consistency for strShuffle() in GLibCRand.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (! file_exists(__DIR__ . '/data/tables/str_shuffle_rand.inc')) {
    die('skip: result table not found.');
}
?>
--FILE--
<?php

[$seed, $base_strings, $shuffled_strings] = include('data/tables/str_shuffle_rand.inc');
$generated_strings = (new \ORNG\GLibCRand($seed))->strShuffle($base_strings);
if ($shuffled_strings !== $generated_strings) {
    die("NG, GLibCRand generated incorrect results.");
}

die('OK, GLibCRand results correctly.');
?>
--EXPECT--
OK, GLibCRand results correctly.
