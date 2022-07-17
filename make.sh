#!/bin/bash
###
# @Author          : lihugang
# @Date            : 2022-07-10 17:13:41
 # @LastEditTime    : 2022-07-10 17:14:03
 # @LastEditors     : lihugang
# @Description     :
 # @FilePath        : \make.sh
# Copyright (c) lihugang
# 长风破浪会有时 直挂云帆济沧海
# There will be times when the wind and waves break, and the sails will be hung straight to the sea.
#  * * *
# 是非成败转头空 青山依旧在 几度夕阳红
# Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
###
#Project: Pig Web Server
#Make File Version: 1.0.0

#project basic info
project_name="Pig Web Server"
project_version="0.0.1 Alpha"
dest_executable_filename="pws"

#print basic info
echo "Project: $project_name"
echo "Version: $project_version"
if [ "$1" == "debug" ]; then
    echo "Current Mode: Debug"
else
    if [ "$1" == "release" ]; then
        echo "Current Mode: Release"
    else
        echo "Error: Unknown Mode: $1"
        echo "Accept: debug,release"
        exit 3
    fi
fi

#check os type
if [ "$OSTYPE" == "msys" -o "$OSTYPE" == "linux" ]; then
    echo "Opreating System Type: $OSTYPE"
else
    echo "Unknown OS: $OSTYPE"
    echo "Only Windows and Linux are supported."
    exit 1
fi

#check gcc exists

gcc -v >/dev/null 2>&1
#not print gcc version, only check gcc exists

# $?(isError) == 0 means no error, gcc exists
if [ $? -eq 0 ]; then
    #gcc exists
    echo "Detecting gcc"
    #do nothing
else
    echo "Error: gcc not found."
    echo "Please install it"
    exit 2
fi

if [ "$OSTYPE" == "msys" ]; then
    #Windows

    #make build directory
    if [ ! -d "build" ]; then
        mkdir build
        #if directory not exists, create it
    fi
    cd build

    echo "Compiling... Please wait a minute."

    #compile file
    if [ "$1" == "debug" ]; then
        #debug mode
        gcc -o "$dest_executable_filename.exe" $(#output file` \
            ../src/index.c
        ) \
            -Wall -Wextra $(#input file` \
                #Warning tips` \
                -O0
            ) \
            -ggdb $(#optimize level` \
                #debug ` \
                -std=c99
            ) \
            -funsigned-char $(#c99 standard` \
                #using unsigned char` \
                -m32
            ) \
            -DDEBUG $(#32-bit executable file` \
                #defined macro`\
                -lws2_32
            ) #static libraries`
    else
        if [ "$1" == "release" ]; then
            #release mode
            gcc -o "$dest_executable_filename.exe" $(#output file` \
                ../src/index.c
            ) \
                -O3 $(#input file`\
                    #optimize level` \
                    -std=c99
                ) \
                -funsigned-char $(#c99 standard` \
                    #using unsigned char` \
                    -m32
                ) \
                "-DRELEASE=\"$project_version\"" $(#32-bit executable file` \
                    #defined macro`\
                    -lws2_32
                ) #static libraries`
        fi
    fi

    if [ $? -eq 0 ]; then
        echo ""
        echo "Your file is in the ./build/ directory"
        echo "Thanks for using $project_name $project_version $1"
    else
        echo ""
        echo "Compile error!"
    fi
fi

if [ "$OSTYPE" == "linux" ]; then
    #Linux

    #make build directory
    if [ ! -d "build" ]; then
        mkdir build
        #if directory not exists, create it
    fi
    cd build

    echo "Compiling... Please wait a minute."

    #compile file
    if [ "$1" == "debug" ]; then
        #debug mode
        gcc -o "$dest_executable_filename" $(#output file` \
            ../src/index.c
        ) \
            -Wall -Wextra $(#input file` \
                #Warning tips` \
                -O0
            ) \
            -ggdb $(#optimize level` \
                #debug ` \
                -std=c99
            ) \
            -funsigned-char $(#c99 standard` \
                #using unsigned char` \
                -m32
            ) \
            -DDEBUG $(#32-bit executable file` \
                #defined macro`\
                -lws2_32
            ) #static libraries`
    else
        if [ "$1" == "release" ]; then
            #release mode
            gcc -o "$dest_executable_filename" $(#output file` \
                ../src/index.c
            ) \
                -O3 $(#input file`\
                    #optimize level` \
                    -std=c99
                ) \
                -funsigned-char $(#c99 standard` \
                    #using unsigned char` \
                    -m32
                ) \
                "-DRELEASE=\"$project_version\"" $(#32-bit executable file` \
                    #defined macro`\
                    -lws2_32
                ) #static libraries`
        fi
    fi

    if [ $? -eq 0 ]; then
        echo ""
        echo "Your file is in the ./build/ directory"
        echo "Thanks for using $project_name $project_version $1"
    else
        echo ""
        echo "Compile error!"
    fi
fi
