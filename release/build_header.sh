#!/bin/bash
set -eo pipefail

FILES="../include/common/types.hpp
../include/common/traits.hpp
../include/common/duration.hpp
../include/common/stringify.hpp
../include/common/streambuf_proxy_omp.hpp
../include/common/streambuf_proxy.hpp
../include/common/assertion_failure.hpp
../include/testsuite/statistics.hpp
../include/testsuite/testcase.hpp
../include/testsuite/testsuite.hpp
../include/testsuite/testsuite_parallel.hpp
../include/testsuite/runner.hpp
../include/testsuite/testmodule.hpp
../include/reporter/reporter.hpp
../include/reporter/xml_reporter.hpp
../include/reporter/console_reporter.hpp
../include/reporter/markdown_reporter.hpp
../include/comparator/comparators.hpp
../include/comparator/less.hpp
../include/comparator/inrange.hpp
../include/comparator/unequals.hpp
../include/comparator/equals.hpp
../include/comparator/greater.hpp
../include/assert.hpp
../include/sctf.hpp"

TARGET="sctf.hpp"

COPYRIGHT="/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */"

echo "" > $TARGET.tmp
for f in $FILES; do
  cat $f >> $TARGET.tmp
done

grep '#include' $TARGET.tmp > .includes
perl -0pi -e 's/#include [<"].*[">]//g' $TARGET.tmp
perl -0pi -e 's%/// .*|/\*[\w\W\n]*?\*/%%g' $TARGET.tmp

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
