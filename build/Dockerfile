FROM morfuse/lib

FROM ubuntu:latest

# install dev tools
RUN apt update && apt install -y ninja-build cmake make clang-12 lld-12

COPY --from=0 "/usr/local/morfuse" "/usr/local/morfuse"

WORKDIR /usr/src/mfuse/sexec

COPY "./CMakeLists.txt" "./"
COPY "./src" "./src"
COPY "./tests" "./tests"
COPY "./thirdparty" "./thirdparty"

RUN mkdir -p ./build && cd ./build \
    && cmake -DCMAKE_C_COMPILER=clang-12 -DCMAKE_CXX_COMPILER=clang++-12 -DCMAKE_INSTALL_PREFIX=/usr/local/morfuse/sexec -Dmorfuse_DIR=/usr/local/morfuse/engine -DCMAKE_BUILD_TYPE=Release -G Ninja ../ \
    && ninja \
    && cmake --install .
