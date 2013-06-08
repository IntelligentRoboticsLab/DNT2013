sudo apt-get install cmake build-essential zlib1g-dev
wget http://www2.informatik.hu-berlin.de/~naoth/ressources/toolchains/NaoTHToolChain_Ubuntu64_453.tar.xz
cd NaoTH2011-light
tar -xvf ../NaoTHToolChain_Ubuntu64_453.tar.xz
rm ../NaoTHToolChain_Ubuntu64_453.tar.xz
cd NaoTHToolChain_Ubuntu64/toolchain_native/extern
mkdir lib
cd ../../
sh setup.sh
