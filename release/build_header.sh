#!/bin/bash
set -eo pipefail

FILES="../include/version.hpp
../include/cpp_meta.hpp
../include/omp.hpp
../include/traits.hpp
../include/duration.hpp
../include/regex.hpp
../include/stringify.hpp
../include/assert/loc.hpp
../include/assert/assertion_failure.hpp
../include/assert/assert.hpp
../include/assert/ordering.hpp
../include/assert/equality.hpp
../include/assert/range.hpp
../include/assert/regex.hpp
../include/test/testcase.hpp
../include/test/streambuf_proxy.hpp
../include/test/statistic.hpp
../include/test/testsuite.hpp
../include/test/testsuite_parallel.hpp
../include/report/reporter.hpp
../include/report/xml_reporter.hpp
../include/report/console_reporter.hpp
../include/report/markdown_reporter.hpp
../include/report/json_reporter.hpp
../include/report/reporter_factory.hpp
../include/config.hpp
../include/cmdline_parser.hpp
../include/runner.hpp
../include/api.hpp
../include/tpp.hpp"

TARGET="tpp.hpp"

COPYRIGHT="/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/"

echo "" > $TARGET.tmp
for f in $FILES; do
  cat $f >> $TARGET.tmp
done

grep '#include' $TARGET.tmp > .includes
perl -0pi -e 's/#include [<"].*[">]//g' $TARGET.tmp
perl -0pi -e 's%//[/<]*? .*|/\*[\w\W\n]*?\*/%%g' $TARGET.tmp

echo "$COPYRIGHT" > $TARGET
echo "#ifndef TPP_RELEASE_TPP_HPP" >> $TARGET
echo "#define TPP_RELEASE_TPP_HPP" >> $TARGET
cat .includes >> $TARGET
cat $TARGET.tmp >> $TARGET
echo "" >> $TARGET
echo "#endif" >> $TARGET

perl -0pi -e 's/#include ".*"//g' $TARGET
if ! command -v clang-format-10 &> /dev/null
then
    clang-format -style=file -i $TARGET
else
    clang-format-10 -style=file -i $TARGET
fi

rm $TARGET.tmp
rm .includes
