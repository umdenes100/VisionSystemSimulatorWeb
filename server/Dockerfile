FROM ubuntu:latest

RUN echo "Installing dependencies..." && \
    apt -y --no-install-recommends update && \ 
    # apt -y --no-install-recommends upgrade && \
    apt install -y --no-install-recommends \
    build-essential \
    git \
    python3-pip \
    nano \
    cmake \
    lsof \
    sudo

RUN echo "Installing cJSON..." && \
	git clone https://github.com/DaveGamble/cJSON.git && \
	cd cJSON && \
	mkdir build && \
	cd build && \
	cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/usr && \
	make && \
	make install

COPY ./server /server

RUN echo "Building randomization binary..." && \
    make -C /server/randomization

RUN pip3 install websockets

WORKDIR /server

CMD ["python3", "-u", "middleware.py"]