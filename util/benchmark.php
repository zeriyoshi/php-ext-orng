<?php

const EXTENSION_NAME = 'orng';

$wait_secs = 5;
$seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
$count = 500000000;
$informations = [
    'Environment' => [
        'OS' => PHP_OS,
    ],
    'PHP' => [
        'Version' => PHP_VERSION,
        'Version ID' => PHP_VERSION_ID,
        'SAPI' => PHP_SAPI,
        'Thread Safe (ZTS)' => PHP_ZTS === 1 ? 'Yes' : 'No',
        'Debug Build' => PHP_DEBUG === 1 ? 'Yes' : 'No',
        'INT Max' => PHP_INT_MAX,
        'INT Min' => PHP_INT_MIN,
    ],
    'Extension' => [
        'Enabled' => extension_loaded(EXTENSION_NAME) ? 'Yes' : 'No',
        'Version' => empty(phpversion(EXTENSION_NAME)) ? 'Unknown' : phpversion(EXTENSION_NAME),
    ],
    'Preferences' => [
        'Wait secs' => $wait_secs,
        'Seed' => $seed,
        'Count' => $count,
    ],
];

echo "--------------------------------------------------\n" .
     "orng Extension Benchmark\n" .
     "--------------------------------------------------\n";
foreach ($informations as $category => $values) {
    echo "${category} informations:\n";
    foreach ($values as $key => $value) {
        echo "\t${key}: ${value}\n";
    }
}
echo "--------------------------------------------------\n";

if (! extension_loaded(EXTENSION_NAME)) {
    echo "ERROR: missing orng extension.\n";
    exit(2);
}

function benchmark(string $category, callable $f) {
    global $wait_secs;

    echo " > ${category}: ";
    sleep($wait_secs);
    $start = microtime(true);
    $f();
    $end = microtime(true);

    echo $end - $start . PHP_EOL;
}

benchmark(
    "native mt_srand / mt_rand performance (count: ${count})",
    function () use ($seed, $count) {
        \mt_srand($seed);
        for ($i = 0; $i < $count; $i++) {
            \mt_rand();
        }
    }
);

foreach ([
    \ORNG\GLibCRand::class,
    \ORNG\XorShift128Plus::class,
    \ORNG\MT19937::class,
    \ORNG\MT19937PHP::class,
] as $class) {
    benchmark(
        "orng ${class} performance (count: ${count})",
        function () use ($class, $seed, $count) {
            $obj = new $class($seed);
            for ($i = 0; $i < $count; $i++) {
                $obj->next();
            }
        }
    );
}
