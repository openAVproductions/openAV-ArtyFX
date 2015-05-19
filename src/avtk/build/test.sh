#!/bin/bash

make

#  --gen-suppressions=all
valgrind --suppressions=valgrind.suppressions --leak-check=full ./avtk
