FROM ubuntu:latest

ENV PYTHONUNBUFFERED 1

RUN echo "Installing dependencies..." && \
    apt -y --no-install-recommends update && \ 
    # apt -y --no-install-recommends upgrade && \
    apt remove --purge python && apt autoremove && \
    apt install -y --no-install-recommends \
    python3.7 \
    python3-pip \
    g++ \
    gcc \
    libc6-dev \
    make \
    wget \
    cmake \
    git \
    nano \
    lsof \
    castxml \
    sudo

RUN echo "Installing cJSON..." && \
	git clone https://github.com/DaveGamble/cJSON.git && \
	cd cJSON && \
	mkdir build && \
	cd build && \
	cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/usr && \
	make && \
	make install

RUN python3.7 -m pip install setuptools
RUN python3.7 -m pip install websockets aiohttp

COPY ./server /server

WORKDIR /server

ENTRYPOINT ["/server/entrypoint.sh"]
