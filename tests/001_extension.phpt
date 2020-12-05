--TEST--
Check extension availability
--FILE--
<?php
if (extension_loaded('orng')) {
    echo 'OK, extension is available.';
} else {
    echo 'NG, extension is NOT available.';
}
?>
--EXPECT--
OK, extension is available.
