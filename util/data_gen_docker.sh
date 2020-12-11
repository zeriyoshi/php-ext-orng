#!/bin/sh

cd "$(dirname "${0}")" || exit

# generate PHP 7.0 (glibc) srand() / rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -it php:7.0-cli "/work/util/data_gen.php" "rand" "/work/tests/data"

# generate PHP 7.0 broken mt_srand() / rand() result table.
docker run --rm -v"$(pwd)/../:/work:cached" -it php:7.0-cli "/work/util/data_gen.php" "mt_php" "/work/tests/data"

# generate PHP 8.0 correctly mt_srand() / rand result table.
docker run --rm -v"$(pwd)/../:/work:cached" -it php:8.0-cli "/work/util/data_gen.php" "mt" "/work/tests/data"
