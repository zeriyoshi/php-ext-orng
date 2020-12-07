--TEST--
Check extension availability
--FILE--
<?php
if (extension_loaded('orng')) {
    die('OK, extension is available.');
}
die('NG, extension is NOT available.');
?>
--EXPECT--
OK, extension is available.
