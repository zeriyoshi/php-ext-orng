#!/usr/bin/env php
<?php

$target = $argv[1] ?? null;
$output_dir = $argv[2] ?? null;

$informations = [
    'Mode' => [
        'Target' => $type ?? 'NULL',
        'Output Dir' => $output_dir ?? 'NULL',
    ],
    'Environment' => [
        'OS' => PHP_OS,
    ],
    'PHP' => [
        'Version' => PHP_VERSION,
        'Version ID' => PHP_VERSION_ID,
        'Thread Safe (ZTS)' => PHP_ZTS === 1 ? 'Yes' : 'No',
    ],
];

echo "--------------------------------------------------\n" .
     "orng Data Generator\n" .
     "--------------------------------------------------\n";
foreach ($informations as $category => $values) {
    echo "${category} informations:\n";
    foreach ($values as $key => $value) {
        echo "\t${key}: ${value}\n";
    }
}
echo "--------------------------------------------------\n";

if ($output_dir === null || ! is_dir($output_dir)) {
    echo "ERROR: Invalid output directory: ${output_dir}\n";
    exit(3);
}

switch ($target) {
    case 'rand':
        exit(rand_generate($output_dir));
    case 'mt':
        exit(mt_generate($output_dir));
    case 'mt_php':
        exit(mt_php_generate($output_dir));
    default:
        echo "ERROR: Invalid target: ${target}\n";
        exit(4);
}

function rand_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.0.0', '>=')) {
        echo "ERROR: libc rand available on 7.0 >= PHP\n";
        return 20;
    }
    $table = [];
    foreach (range(1, 10) as $i) {
        $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
        while (array_key_exists($seed, $table)) {
            $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
        }
        $table[$seed] = [];
        srand($seed);
        foreach (range(1, 10000) as $j) {
            $table[$seed][] = rand();
        }
    }

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'rand.inc'),
        ("<?php return " . var_export($table, true) . ';' )
    );
    return 0;
}

function mt_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1.0', '<')) {
        echo "ERROR: Correctly Melesene Twister available on PHP >= 7.1\n";
        return 20;
    }

    $table = [];
    foreach (range(1, 10) as $i) {
        $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
        while (array_key_exists($seed, $table)) {
            $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
        }
        $table[$seed] = [];
        mt_srand($seed, MT_RAND_MT19937);
        foreach (range(1, 10000) as $j) {
            $table[$seed][] = mt_rand();
        }
    }

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'mt.inc'), 
        ("<?php return " . var_export($table, true) . ';' )
    );
    return 0;
}

function mt_php_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1.0', '>=')) {
        echo "WARNING: Recommended to use PHP 7.1 or lower\n";
        $mt_srand_invoke = function (int $seed): void{
            mt_srand($seed, MT_RAND_MT19937);
        };
    } else {
        $mt_srand_invoke = function (int $seed)/*: void */{
            mt_srand($seed);
        };
    }

    $table = [];
    foreach (range(1, 10) as $i) {
        $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
        while (array_key_exists($seed, $table)) {
            $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
        }
        $table[$seed] = [];
        $mt_srand_invoke($seed);
        foreach (range(1, 10000) as $j) {
            $table[$seed][] = mt_rand();
        }
    }

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'mt_php.inc'), 
        ("<?php return " . var_export($table, true) . ';' )
    );
    return 0;
}
