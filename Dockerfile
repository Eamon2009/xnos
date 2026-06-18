FROM ubuntu:24.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
      build-essential \
      cmake \
      ninja-build \
      git \
      ca-certificates \
      && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -B build \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_TESTS=ON \
      && cmake --build build --parallel \
      && ctest --test-dir build --output-on-failure
FROM ubuntu:24.04 AS dev

ARG DEBIAN_FRONTEND=noninteractive

LABEL org.opencontainers.image.title="xnos"
LABEL org.opencontainers.image.description="Zero-dependency C++17 hardware monitor – dev image"
LABEL org.opencontainers.image.source="https://github.com/Eamon2009/xnos"
LABEL org.opencontainers.image.licenses="MIT"

RUN apt-get update && apt-get install -y --no-install-recommends \
      libstdc++6 \
      ca-certificates \
      build-essential \
      cmake \
      ninja-build \
      git \
      gdb \
      valgrind \
      clang-format \
      && rm -rf /var/lib/apt/lists/*

COPY --from=builder /src/build/iso-kernos /usr/local/bin/xnos
COPY --from=builder /src /workspace/xnos

WORKDIR /workspace/xnos

ENTRYPOINT ["xnos"]
CMD ["--mode", "dashboard", "--test-mode"]