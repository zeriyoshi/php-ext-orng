--TEST--
Check consistency for shuffle() in MT19937MB.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (! file_exists(__DIR__ . '/data/tables/shuffle_mt_mb.inc')) {
    die('skip: result table not found.');
}
?>
--FILE--
<?php

[$seed, $range_min, $range_max, $table] = include('data/tables/shuffle_mt_mb.inc');
$generated_table = range($range_min, $range_max);
(new \ORNG\MT19937MB($seed))->shuffle($generated_table);
for ($i = $range_min; $i < $range_max; $i++) {
    $correct = $table[$i];
    $generated = $generated_table[$i];
    if ($correct !== $generated) {
        die("NG, MT19937MB generated incorrect results. i: ${i} correct: ${correct} generated: ${generated}");
    }
}

die('OK, MT19937MB results correctly.');
?>
--EXPECT--
OK, MT19937MB results correctly.
