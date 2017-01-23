#!/bin/bash

set -e

mkdir -p test_build
pushd test_build >/dev/null
cmake .. -DCMAKE_BUILD_TYPE=GCOV -DDISABLE_QREQ=1 -DDISABLE_REQ=1
make

ROOTFOLDER=$(realpath ..)
LCOVCOMMON="-b ${ROOTFOLDER} -d ."

lcov --zerocounters -d .
lcov ${LCOVCOMMON} -c --initial -o tx.info
./t
lcov ${LCOVCOMMON} --no-checksum -c -o tx.info
lcov -e tx.info "${ROOTFOLDER}/*" -o tx2.info
lcov -r tx2.info "${ROOTFOLDER}/test/*" -o tx3.info
genhtml -prefix $(realpath ..) tx3.info

echo "If everything worked as expected, you should find the report in test_build/index.html"
popd >/dev/null
