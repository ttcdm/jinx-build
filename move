#!/bin/sh

cp /home/titus/test/vinix-test/builds/test/test /home/titus/test/willowOS/limine-c-template-x86-64/test_bin/test
cd /home/titus/test/willowOS/limine-c-template-x86-64/
./package_bin.sh
cd -