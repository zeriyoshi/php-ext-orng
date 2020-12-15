<?php

/** @generate-function-entries */

namespace ORNG;

class XorShift128Plus implements RNGInterface, \Serializable
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function next64(): int {}
    public function range(int $min, int $max): int {}
    public function shuffle(array &$array): bool {}
    public function arrayRand(array $array, int $num = 1): int|string|array {}
    public function strShuffle(string $string): string {}
    public function serialize(): string {}
    public function unserialize(string $serialized): void {}
}
