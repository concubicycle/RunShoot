#!/bin/bash

cppfile="$1.cpp"
hfile="$1.hpp"
project=$2
namespace=${3:-$project}



hguard="__${1^^}_H_"

hpath="./include/${project}/${hfile}"
cpath="./src/$cppfile"

# Create hpp and cpp files
mkdir -p "./include/${project}"

echo "#include \"${project}/${hfile}\"
" > $cpath

 echo "#ifndef $hguard
#define $hguard

namespace $namespace
{
    class $1
    {
    };
}

#endif" > $hpath

# This is super brittle, but whatever
headerlist_set="set(HEADER_LIST"
insert_line="\${PROJECT_SOURCE_DIR}/include/${project}/${hfile}"
sed -i "/${headerlist_set}/a ${insert_line}" ./src/CMakeLists.txt 

sourcelist_set="set(${project^^}_SOURCE_LIST"
c_insert_line="\${PROJECT_SOURCE_DIR}/src/$cppfile"
sed -i "/${sourcelist_set}/a ${c_insert_line}" ./src/CMakeLists.txt 
