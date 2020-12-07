--TEST--
Check pollution variable in XorShift128Plus.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php
$seed = 1000;

new \ORNG\XorShift128Plus($seed);

if ($seed !== 1000) {
    die("NG, variable pollution is detected: ${seed}");
}
die('OK, variable pollution is NOT detected.');
?>
--EXPECT--
OK, variable pollution is NOT detected.
