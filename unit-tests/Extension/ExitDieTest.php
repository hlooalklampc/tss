<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class ExitDieTest extends \PHPUnit_Framework_TestCase
{
    public function testExitDie()
    {
        $phpBinary = constant('PHP_BINARY');
        /* If we use phpdbg, you need to add options -qrr */
        if (defined('PHP_SAPI') && constant('PHP_SAPI') == 'phpdbg') {
            $phpBinary .= ' -qrr';
        }

        $testfile1 = __DIR__ .'/fixtures/exit.php';
        $return1 = `$phpBinary $testfile1`;
        $this->assertSame('', trim($return1));

        $arg = 'Hello World';
        $testfile2 = __DIR__ .'/fixtures/exit_string.php';
        $return2 = `$phpBinary $testfile2 "$arg"`;
        $this->assertSame($arg, trim($return2));

        $testfile3 = __DIR__ .'/fixtures/exit_int.php';
        $intArg = 128;
        $cmd3 = "$phpBinary $testfile3 $intArg";
        exec($cmd3, $out3, $return3);
        $this->assertSame($return3, $intArg);
    }
}
