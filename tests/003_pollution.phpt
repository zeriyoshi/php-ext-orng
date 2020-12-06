--TEST--
Check pollution another objects.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php
const SEED = 50;

$classes = [
    \ORNG\XorShift128Plus::class,
    \ORNG\GLibCRand::class,
];

foreach ($classes as $class) {
    $origin = new $class(SEED);
    $children = [];
    foreach (range(1, 100) as $i) {
        $children[] = new $class(SEED);
    }
    foreach (range(1, 1000) as $i) {
        $next = $origin->next();
        foreach ($children as $child) {
            if ($child->next() !== $next) {
                echo "NG, polltion detected in ${class} (count: ${i})";
                return;
            }
        }
    }
}
echo "OK, pollution is NOT detected.";
?>
--EXPECT--
OK, pollution is NOT detected.
