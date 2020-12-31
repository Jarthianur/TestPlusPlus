# Changelog

## 3

The project is renamed from simple-cpp-test-framework to TestPlusPlus (Test++).
Extended the versioning scheme by revisions.

### 3.0

#### 3.1-0

- added command line argument to set thread count for omp

#### 3.0-0

- major renaming from SCTF to TPP
- major refactoring
- several bugfixes and improvements
- added json reporter
- added experimental atomic execution feature for gcc
- added a default command line parser
- default main now uses command line flags instead of hardcoding
- changed comparators to assertions
  - includes minor API changes for comparisons
  - major internal changes
- changed markdown report format

## 2

The API is redesigned for self registering testsuites.
Basically everything exposed to the user is done using macros.

### 2.1

- assertion failures now contain exception messages
- reports are generated on the fly per testsuite
- type names are now OS dependant
- changed dependency between runner and reporter
- derived type names are properly stringified now
- console report format changed
- testsuite hook functions (setup, etc.) now fail by exceptions

### 2.0

- redesign of API
- major refactoring
- changes in report formats
- function asserts return their products
- several bugfixes

## 1.x

A rundimentary version of this framework, where the API was strict C++ code and macros where only used for asserts, to hide `__LINE__` and `__FILE__`.
_There are several issues in this version, please do not use it anymore._
