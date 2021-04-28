#!/usr/bin/env sh

dir=$(mktemp -d)

cat > "$dir/CMakeLists.txt" << EndOfMessage
set(REQUIRED_QT_VERSION 5.15.0)

project(Tok LANGUAGES CXX)

cmake_minimum_required(VERSION 3.19)

find_package(KF5Kirigami2 REQUIRED)
find_package(KF5I18n REQUIRED)
find_package(KF5Notifications REQUIRED)
find_package(Td 1.7.3 REQUIRED)

add_executable(tok null.cpp)

target_link_libraries(tok
  PUBLIC
    KF5::Kirigami2 KF5::I18n KF5::Notifications Td::TdStatic
)
EndOfMessage

touch "$dir/null.cpp"

cd "$dir"
mkdir _build
cd _build
mkdir -p ".cmake/api/v1/query"
touch ".cmake/api/v1/query/codemodel-v2"
nohup cmake .. >/dev/null 2>&1

jq -r ".link.commandFragments[].fragment" .cmake/api/v1/reply/target-*.json

echo "===="

jq -r ".compileGroups[].includes[].path" .cmake/api/v1/reply/target-*.json
