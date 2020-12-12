--TEST--
Check MT19937 modulo bias on 64-bit machines.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php
$rng = new \ORNG\MT19937(1234567890);
$total = 10000;
$max = 0x66666666;
$halves[0] = $halves[1] = 0;
for ($i = 0; $i < $total; $i++) {
    $halves[($rng->range(0, $max - 1) >> 1) & 1]++;
}
printf("%.1f%% vs. %.1f%%\n", 100. * $halves[0] / $total, 100. * $halves[1] / $total);

$rng = new \ORNG\MT19937(1234567890);
$total = 10000;
$max = 0x66666666;
$halves[0] = $halves[1] = 0;
for ($i = 0; $i < $total; $i++) {
    $halves[$rng->range(0, $max - 1) / ($max / 2)]++;
}
printf("%.1f%% vs. %.1f%%\n", 100. * $halves[0] / $total, 100. * $halves[1] / $total);
$rng = new \ORNG\MT19937MB(1234567890);
$total = 10000;
$max = 0x66666666;
$halves[0] = $halves[1] = 0;
for ($i = 0; $i < $total; $i++) {
    $halves[($rng->range(0, $max - 1) >> 1) & 1]++;
}
printf("%.1f%% vs. %.1f%%\n", 100. * $halves[0] / $total, 100. * $halves[1] / $total);

$rng = new \ORNG\MT19937MB(1234567890);
$total = 10000;
$max = 0x66666666;
$halves[0] = $halves[1] = 0;
for ($i = 0; $i < $total; $i++) {
    $halves[$rng->range(0, $max - 1) / ($max / 2)]++;
}
printf("%.1f%% vs. %.1f%%\n", 100. * $halves[0] / $total, 100. * $halves[1] / $total);
$rng = new \ORNG\MT19937PHP(1234567890);
$total = 10000;
$max = 0x66666666;
$halves[0] = $halves[1] = 0;
for ($i = 0; $i < $total; $i++) {
    $halves[($rng->range(0, $max - 1) >> 1) & 1]++;
}
printf("%.1f%% vs. %.1f%%\n", 100. * $halves[0] / $total, 100. * $halves[1] / $total);

$rng = new \ORNG\MT19937PHP(1234567890);
$total = 10000;
$max = 0x66666666;
$halves[0] = $halves[1] = 0;
for ($i = 0; $i < $total; $i++) {
    $halves[$rng->range(0, $max - 1) / ($max / 2)]++;
}
printf("%.1f%% vs. %.1f%%\n", 100. * $halves[0] / $total, 100. * $halves[1] / $total);
?>
--EXPECT--
49.5% vs. 50.5%
50.5% vs. 49.5%
49.7% vs. 50.3%
60.5% vs. 39.5%
39.8% vs. 60.2%
50.2% vs. 49.8%