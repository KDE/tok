#!/usr/bin/env bash

bail() {
	echo $@
	exit 1
}

has() {
  type -p "$1" >/dev/null
}

has cmake || bail "CMake is missing; please install CMake"
has jq || bail "jq is missing; please install jq"

dir=$(mktemp -d)

cat > "$dir/CMakeLists.txt" << EndOfMessage
set(REQUIRED_QT_VERSION 5.15.0)

project(Tok LANGUAGES CXX)

cmake_minimum_required(VERSION 3.5)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH \${ECM_MODULE_PATH})

$1

add_executable(tok null.cpp)

target_link_libraries(tok
  PUBLIC
    $2
)

EndOfMessage

touch "$dir/null.cpp"

cd "$dir"
mkdir _build
cd _build
mkdir -p ".cmake/api/v1/query"
touch ".cmake/api/v1/query/codemodel-v2"
cmake .. 2>&1 >nohup.out
retval=$?

if [ $retval -ne 0 ]; then
    cat nohup.out
    exit $retval
fi

jq -r ".link.commandFragments[].fragment" .cmake/api/v1/reply/target-*.json || bail "Check out the logs in" $PWD

echo "===="

jq -r ".compileGroups[].includes[].path" .cmake/api/v1/reply/target-*.json || bail "Check out the logs in" $PWD
