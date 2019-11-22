#!/usr/bin/env sh

set -e

cpplint --linelength=80 --counting=detailed $(find src -path "*/*/model" -prune -or -name "*.h" -or -name "*.cc")
cpplint --linelength=80 --counting=detailed $(find include -name "*.h" -or -name "*.cc")
cpplint --linelength=80 --counting=detailed $(find test -name "*.h" -or -name "*.cc")
