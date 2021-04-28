FROM alpine as build
RUN apk update && apk add --no-cache  build-base cmake libcurl curl-dev make
COPY src /src
RUN mkdir /src/build
RUN cd /src/build
ENV CXXFLAGS="-DNOPROGRESS=1"
RUN cmake -DCMAKE_BUILD_TYPE=Release -DPFGPLUGINS=ON ../src
RUN make -j `grep -c proc /proc/cpuinfo`

FROM alpine as dqm
MAINTAINER Grigory Latyshev
RUN apk update && apk add --no-cache libcurl libstdc++
COPY --from=build /libdqmcpp.so /usr/lib/
COPY --from=build /dqm /usr/bin/
RUN mkdir /results
WORKDIR /results
ENTRYPOINT ["/usr/bin/dqm"]

FROM alpine as findDataset
MAINTAINER Grigory Latyshev
RUN apk update && apk add --no-cache libcurl libstdc++
COPY --from=build /libdqmcpp.so /usr/lib/
COPY --from=build /findDataset /usr/bin/
ENTRYPOINT ["/usr/bin/findDataset"]

FROM alpine as onlineRuns
MAINTAINER Grigory Latyshev
RUN apk update && apk add --no-cache libcurl libstdc++
COPY --from=build /libdqmcpp.so /usr/lib/
COPY --from=build /onlineRuns /usr/bin/

ENTRYPOINT ["/usr/bin/onlineRuns"]