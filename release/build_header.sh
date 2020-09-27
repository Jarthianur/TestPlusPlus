#!/bin/bash
set -eo pipefail

FILES="../include/cpp_meta.hpp
../include/loc.hpp
../include/traits.hpp
../include/duration.hpp
../include/regex.hpp
../include/stringify.hpp
../include/assertion_failure.hpp
../include/testsuite/testcase.hpp
../include/testsuite/streambuf_proxy_omp.hpp
../include/testsuite/streambuf_proxy.hpp
../include/testsuite/statistic.hpp
../include/testsuite/testsuite.hpp
../include/testsuite/testsuite_parallel.hpp
../include/reporter/reporter.hpp
../include/reporter/xml_reporter.hpp
../include/reporter/console_reporter.hpp
../include/reporter/markdown_reporter.hpp
../include/reporter/reporter_factory.hpp
../include/cmdline_parser.hpp
../include/runner.hpp
../include/comparator/comparator.hpp
../include/comparator/ordering.hpp
../include/comparator/equality.hpp
../include/comparator/range.hpp
../include/comparator/regex.hpp
../include/assert.hpp
../include/api.hpp
../include/sctf.hpp"

TARGET="sctf.hpp"

COPYRIGHT="/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/"

echo "" > $TARGET.tmp
for f in $FILES; do
  cat $f >> $TARGET.tmp
done

grep '#include' $TARGET.tmp > .includes
perl -0pi -e 's/#include [<"].*[">]//g' $TARGET.tmp
perl -0pi -e 's%//[/<]*? .*|/\*[\w\W\n]*?\*/%%g' $TARGET.tmp

echo "$COPYRIGHT" > $TARGET
echo "#ifndef SCTF_RELEASE_SCTF_HPP" >> $TARGET
echo "#define SCTF_RELEASE_SCTF_HPP" >> $TARGET
cat .includes >> $TARGET
cat $TARGET.tmp >> $TARGET
echo "" >> $TARGET
echo "#endif" >> $TARGET

perl -0pi -e 's/#include ".*"//g' $TARGET
clang-format -style=file -i $TARGET

rm $TARGET.tmp
rm .includes
