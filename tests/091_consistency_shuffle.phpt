--TEST--
Check consistency for shuffle() in GLibCRand.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (! file_exists('data/tables/shuffle_rand.inc')) {
    die('skip: result table not found.');
}
?>
--FILE--
<?php

[$seed, $range_min, $range_max, $table] = include('data/tables/shuffle_rand.inc');
$generated_table = range($range_min, $range_max);
(new \ORNG\GLibCRand($seed))->shuffle($generated_table);
for ($i = $range_min; $i < $range_max; $i++) {
    $correct = $table[$i];
    $generated = $generated_table[$i];
    if ($correct !== $generated) {
        die("NG, GLibCRand generated incorrect results. i: ${i} correct: ${correct} generated: ${generated}");
    }
}

die('OK, GLibCRand results correctly.');
?>
--EXPECT--
OK, GLibCRand results correctly.
