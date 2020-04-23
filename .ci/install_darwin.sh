#!/usr/bin/env bash

set -ev

OS=darwin
ARCH=x86_64

# install bazel
GH_BASE="https://github.com/bazelbuild/bazel/releases/download/$V_BAZEL"
GH_ARTIFACT="bazel-$V_BAZEL-installer-$OS-$ARCH.sh"
URL="$GH_BASE/$GH_ARTIFACT"
echo $URL
wget -O install_bazel.sh $URL
chmod +x install_bazel.sh
./install_bazel.sh --user
rm -f install_bazel.sh

# install clang-format
 brew install clang-format
 clang-format --version
