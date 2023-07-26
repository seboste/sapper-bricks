FROM ubuntu:latest

RUN apt-get update
RUN apt-get -y upgrade

#compiler & dev tools
RUN apt-get install -y gcc clang build-essential gdb clang-format cmake git make curl

#conan
RUN apt-get install -y python3 python3-pip
RUN pip install conan==1.60.1
RUN conan profile new default --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 default

#sapper
RUN apt-get install wget
RUN wget https://go.dev/dl/go1.20.6.linux-amd64.tar.gz
RUN rm -rf /usr/local/go 
RUN tar -C /usr/local -xzf go1.20.6.linux-amd64.tar.gz 
RUN rm go1.20.6.linux-amd64.tar.gz
ENV PATH=$PATH:/usr/local/go/bin
ENV GOBIN=/usr/local/go/bin
RUN go install github.com/seboste/sapper@latest
