FROM mariadb:latest
ENV MARIADB_ROOT_PASSWORD=pass
RUN set -x \
    && apt-get update \
    && apt-get install --no-install-recommends --no-install-suggests -y \
        vim; \
    rm -rf /var/lib/apt/lists/*