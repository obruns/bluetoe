FROM jenkins/jnlp-slave

USER root

RUN apt update && \
    apt install --assume-yes cmake build-essential libboost-all-dev clang ninja-build

USER jenkins
