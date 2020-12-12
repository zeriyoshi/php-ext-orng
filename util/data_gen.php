#!/usr/bin/env php
<?php

$target = $argv[1] ?? null;
$output_dir = $argv[2] ?? null;

$informations = [
    'Mode' => [
        'Target' => $target ?? 'NULL',
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
    case 'shuffle_rand':
        exit(shuffle_rand_generate($output_dir));
    case 'array_rand_rand':
        exit(array_rand_rand_generate($output_dir));
    case 'str_shuffle_rand':
        exit(str_shuffle_rand_generate($output_dir));
    case 'mt_php':
        exit(mt_php_generate($output_dir));
    case 'mt':
        exit(mt_generate($output_dir));
    case 'shuffle_mt':
        exit(shuffle_mt_generate($output_dir));
    case 'array_rand_mt':
        exit(array_rand_mt_generate($output_dir));
    case 'str_shuffle_mt':
        exit(str_shuffle_mt_generate($output_dir));
    default:
        echo "ERROR: Invalid target: ${target}\n";
        exit(4);
}

function php_wrap(string $php_code): string {
    return "<?php return ${php_code};";
}

function rand_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1', '>')) {
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
        php_wrap(var_export($table, true))
    );
    return 0;
}

function shuffle_rand_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1', '>=')) {
        echo "ERROR: libc rand available on 7.0 >= PHP\n";
        return 20;
    }
    $range_min = 0;
    $range_max = 99999;
    $table = range($range_min , $range_max);
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    \srand($seed);
    \shuffle($table);
    $table = [
        0 => $seed,
        1 => $range_min,
        2 => $range_max,
        3 => $table,
    ];

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'shuffle_rand.inc'),
        php_wrap(var_export($table, true))
    );
    return 0;
}

function array_rand_rand_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1', '>=')) {
        echo "ERROR: libc rand available on 7.0 >= PHP\n";
        return 20;
    }
    $range_min = 0;
    $range_max = 999;
    $base_array = range($range_min , $range_max);
    $pick_array_key = [];
    $multi_pick_array_keys = [];
    $multi_pick_num = 3;
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    \srand($seed);
    foreach ($base_array as $i) {
        $pick_array_key[] = \array_rand($base_array, 1);
    }
    \srand($seed);
    foreach ($base_array as $i) {
        $multi_pick_array_keys[] = \array_rand($base_array, $multi_pick_num);
    }

    $table = [
        0 => $seed,
        1 => $range_min,
        2 => $range_max,
        3 => $pick_array_key,
        4 => $multi_pick_array_keys,
        5 => $multi_pick_num,
    ];

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'array_rand_rand.inc'),
        php_wrap(var_export($table, true))
    );
    return 0;    
}

function str_shuffle_rand_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1', '>=')) {
        echo "ERROR: libc rand available on 7.0 >= PHP\n";
        return 20;
    }
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    $base_strings = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.';
    \srand($seed);
    $shuffled_strings = \str_shuffle($base_strings);
    $table = [
        0 => $seed,
        1 => $base_strings,
        2 => $shuffled_strings,
    ];

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'str_shuffle_rand.inc'),
        php_wrap(var_export($table, true))
    );
    return 0;
}

function mt_php_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.1', '>=')) {
        echo "WARNING: Recommended to use PHP 7.0 or lower\n";
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
        php_wrap(var_export($table, true))
    );
    return 0;
}

function mt_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.2', '<')) {
        echo "ERROR: Correctly Mersenne Twister available on PHP >= 7.2\n";
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
        php_wrap(var_export($table, true))
    );
    return 0;
}

function shuffle_mt_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.2', '<')) {
        echo "ERROR: PHP 7.1 and lower implementation is not Mersenne Twister.\n";
        return 20;
    }
    $range_min = 0;
    $range_max = 99999;
    $table = range($range_min , $range_max);
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    \mt_srand($seed);
    \shuffle($table);
    $table = [
        0 => $seed,
        1 => $range_min,
        2 => $range_max,
        3 => $table,
    ];

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'shuffle_mt.inc'),
        php_wrap(var_export($table, true))
    );
    return 0;
}

function array_rand_mt_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.2', '<')) {
        echo "ERROR: PHP 7.1 and lower implementation is not Mersenne Twister.\n";
        return 20;
    }
    $range_min = 0;
    $range_max = 999;
    $base_array = range($range_min , $range_max);
    $pick_array_key = [];
    $multi_pick_array_keys = [];
    $multi_pick_num = 3;
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    \mt_srand($seed);
    foreach ($base_array as $i) {
        $pick_array_key[] = \array_rand($base_array, 1);
    }
    \mt_srand($seed);
    foreach ($base_array as $i) {
        $multi_pick_array_keys[] = \array_rand($base_array, $multi_pick_num);
    }

    $table = [
        0 => $seed,
        1 => $range_min,
        2 => $range_max,
        3 => $pick_array_key,
        4 => $multi_pick_array_keys,
        5 => $multi_pick_num,
    ];

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'array_rand_mt.inc'),
        php_wrap(var_export($table, true))
    );
    return 0;    
}

function str_shuffle_mt_generate(string $output_dir): int {
    if (version_compare(PHP_VERSION, '7.3', '<')) {
        echo "ERROR: PHP 7.2 and lower implementation is not Mersenne Twister.\n";
        return 20;
    }
    $seed = random_int(PHP_INT_MIN, PHP_INT_MAX);
    $base_strings = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.';
    \mt_srand($seed);
    $shuffled_strings = \str_shuffle($base_strings);
    $table = [
        0 => $seed,
        1 => $base_strings,
        2 => $shuffled_strings,
    ];

    file_put_contents(
        ($output_dir . DIRECTORY_SEPARATOR . 'str_shuffle_mt.inc'),
        php_wrap(var_export($table, true))
    );
    return 0;
}
