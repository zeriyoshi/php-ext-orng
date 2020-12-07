<?php

/** @generate-function-entries */

namespace ORNG;

interface RNGInterface
{
    public function __construct(int $seed);
    public function next(): int;
    public function range(int $min, int $max): int;
}