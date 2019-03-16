FROM ubuntu:latest

RUN echo "Installing dependencies." && \
    apt -y --no-install-recommends update && \ 
    # apt -y --no-install-recommends upgrade && \
    apt install -y --no-install-recommends \
    build-essential \
    git \
    python3-pip \
    nano \
    cmake

RUN pip3 install websockets

WORKDIR /server

CMD ["sleep", "infinity"]
