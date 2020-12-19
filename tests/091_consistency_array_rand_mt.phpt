--TEST--
Check consistency for arrayRand() in MT19937.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (! file_exists('data/tables/array_rand_mt.inc')) {
    die('skip: result table not found.');
}
?>
--FILE--
<?php

[$seed, $range_min, $range_max, $pick_array_key, $multi_pick_array_keys, $multi_pick_num] = include('data/tables/array_rand_mt.inc');
$generated_array = range($range_min, $range_max);
$rng = new \ORNG\MT19937($seed);
foreach ($pick_array_key as $i => $correct) {
    $generated = $rng->arrayRand($generated_array, 1);
    if ($generated !== $correct) {
        die("NG, generated incorrect results.  i: ${i} correct: ${correct} generated: ${generated}");
    }
}
$rng = new \ORNG\MT19937($seed);
foreach ($multi_pick_array_keys as $corrects) {
    $generated = $rng->arrayRand($generated_array, $multi_pick_num);
    for ($i = 0; $i < count($generated); $i++) {
        if ($corrects[$i] !== $generated[$i]) {
            die("NG, generated incorrect results in multi pick. correct: {$corrects[$i]} generated: {$generated[$i]}");
        }
    }
} 

die('OK, MT19937 results correctly.');
?>
--EXPECT--
OK, MT19937 results correctly.
