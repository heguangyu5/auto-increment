<?php
/**

CREATE DATABASE test;
use test;
CREATE TABLE `test_ai` (
  `ai` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`ai`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

*/

function runTest()
{
    $pdo = new PDO('mysql:host=localhost;dbname=test', 'root', '123456');
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $i = 100;
    while ($i--) {
        $j = 1000;
        $values = array();
        while ($j--) {
            $values[] = '(' . trim(file_get_contents('/proc/auto-increment')) . ')';
        }
        $values = implode(',', $values);
        $pdo->exec("INSERT INTO test_ai(ai) VALUES $values");
    }
}

$count = 0;
for ($i = 0; $i < 100; $i++) {
    $pid = pcntl_fork();
    if ($pid == -1) {
        die("fork error\n");
    }
    if ($pid == 0) { // child
        runTest();
        exit;
    }
    // parent
    $count++;
}

echo "$count children runing\n";
while ($count) {
    $pid = pcntl_wait($status);
    if ($pid > 0) {
        $count--;
    }
    echo  "$count children runing\n";
}
