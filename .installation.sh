#!/bin/bash
ubuntu_version=`lsb_release -rs | sed 's/\.//'`

install_common_dependencies()
{
    # # install most dependencies via apt-get
    ${SUDO} apt-get -y update
    ${SUDO} apt-get -y upgrade

    # We explicitly set the C++ compiler to g++, the default GNU g++ compiler.
    # This is needed because we depend on system-installed libraries built
    # with g++ and linked against libstdc++. In case `c++` corresponds to
    # `clang++`, code will not build, even
    # if we would pass the flag `-stdlib=libstdc++` to `clang++`.

    ${SUDO} apt-get -y install g++ pkg-config libboost-all-dev
    ${SUDO} apt-get -y install libboost-serialization-dev libboost-filesystem-dev
    ${SUDO} apt-get -y install libboost-system-dev libboost-program-options-dev
    ${SUDO} apt-get -y install libboost-test-dev libeigen3-dev libode-dev
    ${SUDO} apt-get -y install wget libyaml-cpp-dev
    ${SUDO} apt-get -y install git
    ${SUDO} apt-get -y install build-essential
    ${SUDO} apt-get -y install cmake
    ${SUDO} apt-get -y install libx11-dev
    ${SUDO} apt-get -y install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev
    ${SUDO} apt-get -y install libxrandr-dev
    ${SUDO} apt-get -y install libxi-dev
    ${SUDO} apt-get -y install libxmu-dev
    ${SUDO} apt-get -y install libblas-dev
    ${SUDO} apt-get -y install libxinerama-dev
    ${SUDO} apt-get -y install libxcursor-dev

    export CXX=g++
    export MAKEFLAGS="-j `nproc`"
}

install_common_dependencies