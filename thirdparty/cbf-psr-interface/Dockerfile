FROM artefact.skao.int/ska-tango-images-pytango-builder:9.3.10 as buildenv
FROM artefact.skao.int/ska-tango-images-pytango-runtime:9.3.10

# create ipython profile to so that itango doesn't fail if ipython hasn't run yet
RUN ipython profile create
ENV PATH=/home/tango/.local/bin:$PATH

# install castxml (used by pygccxml)
RUN [ ! -z "$http_proxy" ] && \
        echo "Acquire::http::Proxy \"$http_proxy\";" | sudo tee /etc/apt/apt.conf.d/30proxy ||  \
        echo "no proxy" && \
    export DEBIAN_FRONTEND=noninteractive && sudo apt update && \
    sudo apt install -y g++ castxml && \
    sudo apt clean && sudo rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN pip install -e . --extra-index-url https://nexus.engageska-portugal.pt/repository/pypi/simple

#ENTRYPOINT ["/app/entrypoint.sh"]
#CMD ["low_cbf_subarray", "LowCbfSubarray-default-instance"]
