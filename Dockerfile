FROM  gcc:11.3.0

RUN apt-get update && \
    apt-get install -y make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /sharedmem

COPY . /sharedmem

CMD [ "make", "all" ]
