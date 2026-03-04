#!/bin/bash
export SYSTEMC_HOME=/home/rr/systemc/systemc-3.0.2/build/
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64:$LD_LIBRARY_PATH
exec "$@"

g++ -o systemc_socket systemc-socket.cpp \
    -I$SYSTEMC_HOME/include \
    -L$SYSTEMC_HOME/lib-linux64 -lsystemc \
    -Wl,-rpath,$SYSTEMC_HOME/lib-linux64 \
    -DSC_ALLOW_DEPRECATED_IEEE_API
