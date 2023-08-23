FROM morfuse/lib

# The base image already has these tools installed
#RUN apt update && apt install -y ninja-build cmake make clang-15 lld-15

WORKDIR /usr/src/mfuse/sexec

COPY "./CMakeLists.txt" "./"
COPY "./src" "./src"
COPY "./tests" "./tests"
COPY "./thirdparty" "./thirdparty"

RUN mkdir -p ./build && cd ./build \
    && cmake -DCMAKE_C_COMPILER=clang-15 -DCMAKE_CXX_COMPILER=clang++-15 -DCMAKE_INSTALL_PREFIX=/usr/local/morfuse/sexec -Dmorfuse_DIR=/usr/local/morfuse/engine -DCMAKE_BUILD_TYPE=Release -G Ninja ../ \
    && ninja \
    && cmake --install .
