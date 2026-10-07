FROM alpine:3.21 AS builder

RUN apk add --no-cache \
    build-base \
    cmake \
    ninja

WORKDIR /app

COPY . .

RUN cmake -B build -S . \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
    && cmake --build build --config Release \
    && strip --strip-unneeded build/kvcache

FROM alpine:3.21 AS runner

RUN addgroup -S -g 10001 appgroup \
    && adduser -S -u 10001 -G appgroup -h /app -s /sbin/nologin appuser

RUN apk add --no-cache \
    libstdc++ \
    libgcc \
    ca-certificates \
    tzdata

WORKDIR /app

ARG PORT=7379
ARG PERIODIC_SEC=1

ENV PORT=${PORT} \
    PERIODIC_SEC=${PERIODIC_SEC}

EXPOSE ${PORT}

COPY --chown=appuser:appgroup --from=builder /app/build/kvcache /app/kvcache

USER appuser:appgroup

ENTRYPOINT ["/app/kvcache"]