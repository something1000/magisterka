set -uex

eval "$(conda shell.bash hook)"
conda create --name clang_compiler --yes
conda activate clang_compiler
conda install -c conda-forge fasttext --yes
conda install cmake --yes

WORKDIR=`pwd`
COMPILERS_PATH=$WORKDIR/compilers
APPS_PATH=$WORKDIR/apps
GCC_PATH=$COMPILERS_PATH/gcc_compiler
CLANG_PATH=$COMPILERS_PATH/clang_compiler
UTILS_PATH=$WORKDIR/utils


echo "====Preparing folder structure====="

rm -rf $COMPILERS_PATH
rm -rf $GCC_PATH
rm -rf $APPS_PATH
rm -rf $UTILS_PATH
mkdir -p $UTILS_PATH
mkdir -p $APPS_PATH
mkdir -p $GCC_PATH
mkdir -p $CLANG_PATH

cd $UTILS_PATH

echo "${APPS_PATH}/include"
echo "=====downloading & installing libefi dependency====="
git clone https://github.com/wolfgangst/libelf
cd libelf
./configure --prefix=$APPS_PATH
make -j14
make install
cd ..


echo "=====Downloading & installing libffi dependency====="
wget https://github.com/libffi/libffi/releases/download/v3.3/libffi-3.3.tar.gz
tar xf libffi-3.3.tar.gz
cd libffi-3.3
./configure --prefix=$APPS_PATH
make -j14
make install
cd ..


# # ##################################################
# # ################################################## CLANG
# # ##################################################


echo "----Build LLVM with support for offloading to NVIDIA GPUs.-----"

cd $CLANG_PATH
git clone https://github.com/llvm/llvm-project.git --branch llvmorg-11.1.0 llvm
cd llvm
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=$APPS_PATH \
    -DLIBOMPTARGET_DEP_LIBELF_INCLUDE_DIR:PATH=$APPS_PATH/include \
    -DLIBOMPTARGET_DEP_LIBELF_LIBRARIES:PATH=$APPS_PATH/lib/libelf.so \
    -DLIBOMPTARGET_DEP_LIBFFI_INCLUDE_DIR:PATH=$APPS_PATH/include \
    -DLIBOMPTARGET_DEP_LIBFFI_LIBRARIES:PATH=$APPS_PATH/lib/libffi.so \
    -DLLVM_ENABLE_PROJECTS="clang;openmp" ../llvm/
make -j14
make install

# ##################################################
# ################################################## GCC
# ##################################################

echo "---- Build GCC -----"
# #
conda create --name gcc_compiler --yes
conda activate gcc_compiler
conda install -c conda-forge fasttext --yes
conda install cmake --yes
conda install flex --yes


cd $GCC_PATH
wget -c https://github.com/gcc-mirror/gcc/archive/refs/tags/releases/gcc-10.2.0.tar.gz -O gcc.tar.gz
tar xf gcc.tar.gz
mv gcc-releases-gcc-10.2.0 gcc
cd gcc
./contrib/download_prerequisites
cd ..

# Build GCC
rm -rf build-gcc
mkdir -p build-gcc
cd build-gcc
../gcc/configure \
    --build=x86_64-linux-gnu \
    --host=x86_64-linux-gnu \
    --target=x86_64-linux-gnu \
    --enable-checking=release \
    --enable-shared                                   \
    --disable-multilib                                \
    --enable-languages="c,c++,fortran,lto"            \
    --prefix=$APPS_PATH
make -j14
make install

#export PATH=$APPS_PATH/bin:$PATH
#export LD_LIBRARY_PATH=$APPS_PATH/lib:$LD_LIBRARY_PATH
