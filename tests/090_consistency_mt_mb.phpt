--TEST--
Check consistency for MT19937MB.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
?>
--FILE--
<?php

[$seed, $min, $max, $values] = include('data/tables/mt_mb.inc');
$rng = new \ORNG\MT19937MB($seed);
foreach ($values as $value) {
    $next = $rng->range($min, $max);
    if ($next !== $value) {
        die("NG, MT19937MB wrong result. seed: ${seed} correct: ${value} result: ${next}");
    }
}

die('OK, MT19937MB results correctly.');
--EXPECT--
OK, MT19937MB results correctly.
