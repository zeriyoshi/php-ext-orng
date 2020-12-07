--TEST--
Check pollution another objects in MT19937.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php
const SEED = 50;

$rng1_1 = new \ORNG\MT19937(SEED);
$rng1_2 = new \ORNG\MT19937(SEED);
$rng2_1 = new \ORNG\MT19937PHP(SEED);
$rng2_2 = new \ORNG\MT19937PHP(SEED);

foreach (range(1, 10000) as $i) {
    $rng1_next = $rng1_1->next();
    $rng2_next = $rng2_1->next();
    if ($rng2_next !== $rng2_2->next() || $rng1_next !== $rng1_2->next()) {
        die('NG, MT pollution is detected.');
    }
}
die('OK, MT pollution is NOT detected.');
?>
--EXPECT--
OK, MT pollution is NOT detected.
