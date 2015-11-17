#!/bin/bash

lcov --directory . --zerocounters
./Test
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/test/*' '/usr/*' '/gtest/*' '/include/*' --output-file coverage.info
lcov --list coverage.info