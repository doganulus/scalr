FROM ubuntu:22.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        sudo \
        git \
        build-essential \
        tar curl wget zip unzip gnupg2 \
        cmake \
        ninja-build \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        clang \
        clang-format \
        clang-tidy \
        clang-tools \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 --branch v3.3.1 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
    -DBUILD_TESTING=OFF \
    -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2

# Add a container user (good practice)
RUN groupadd work -g 1000 \
   && useradd -ms /bin/bash scalr -g 1000 -u 1000 \
   && printf "scalr:scalr" | chpasswd \
   && printf "scalr ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

USER scalr

ENV PATH "/home/scalr/.local/bin:${PATH}"
