#!/bin/sh

cd "$(dirname "${0}")" || exit

# generate PHP 7.0 (glibc) srand() / rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.0-cli "/work/util/data_gen.php" "rand" "/work/tests/data/tables"

# generate PHP 7.0 (glibc) srand() based shuffle() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.0-cli "/work/util/data_gen.php" "shuffle_rand" "/work/tests/data/tables"

# generate PHP 7.0 (glibc) srand() based array_rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.0-cli "/work/util/data_gen.php" "array_rand_rand" "/work/tests/data/tables"

# generate PHP 7.0 (glibc) srand() based str_shuffle() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.0-cli "/work/util/data_gen.php" "str_shuffle_rand" "/work/tests/data/tables"

# generate PHP 7.0 broken mt_srand() / rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.0-cli "/work/util/data_gen.php" "mt_php" "/work/tests/data/tables"

# generate PHP 8.0 correctly mt_srand() / rand result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:8.0-cli "/work/util/data_gen.php" "mt" "/work/tests/data/tables"

# generate PHP 7.2 mt_srand() based shuffle() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.2-cli "/work/util/data_gen.php" "shuffle_mt" "/work/tests/data/tables"

# generate PHP 7.2 mt_srand() based array_rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.2-cli "/work/util/data_gen.php" "array_rand_mt" "/work/tests/data/tables"

# generate PHP 7.3 mt_srand() based str_shuffle() result table.
# Note: PHP 7.2 and lower implementation based old srand().
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.3-cli "/work/util/data_gen.php" "str_shuffle_mt" "/work/tests/data/tables"

# generate PHP 7.1 modulo-biased mt_srand() / rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.1-cli "/work/util/data_gen.php" "mt_mb" "/work/tests/data/tables"

# generate PHP 7.1 modulo-biased mt_srand() based shuffle() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.1-cli "/work/util/data_gen.php" "shuffle_mt_mb" "/work/tests/data/tables"

# generate PHP 7.1 modulo-biased mt_srand() based array_rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -i php:7.1-cli "/work/util/data_gen.php" "array_rand_mt_mb" "/work/tests/data/tables"
