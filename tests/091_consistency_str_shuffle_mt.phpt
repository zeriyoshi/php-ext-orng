--TEST--
Check consistency for strShuffle() in MT19937.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
?>
--FILE--
<?php

[$seed, $base_strings, $shuffled_strings] = include('data/tables/str_shuffle_mt.inc');
$generated_strings = (new \ORNG\MT19937($seed))->strShuffle($base_strings);
if ($shuffled_strings !== $generated_strings) {
    die("NG, MT19937 generated incorrect results.");
}

die('OK, MT19937 results correctly.');
--EXPECT--
OK, MT19937 results correctly.
