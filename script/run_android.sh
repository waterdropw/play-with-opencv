#!/usr/bin/env bash -e

echo "usage: $0 <ip_addr> [usb_state]"

wifi_ip=$1
usb_state=$2

if [[ -z "${wifi_ip}" || -z "${usb_state}" ]]; then
    echo "usage: $0 <ip_addr> [usb_state]"
    exit -1
fi


export ARCH_ABI=arm64-v8a
#arm64-v8a #

function RSYNC() {
    rsync -aP  --copy-links -e "$SSH" "$@"
}

function RSYNC_DEL() {
    rsync -aP  --delete --copy-links -e "$SSH" "$@"
}

PORT=${PORT:-8022}
HOST=${HOST:-${wifi_ip}}
ARCH=${ARCH:-${ARCH_ABI}}

SSH="ssh $SSH_OPT -p $PORT -o StrictHostKeyChecking=no"

user="perf-$ARCH"


echo "========================================"
echo "Rebuild ..."
./script/build.sh android arm64


echo "========================================"
echo "Setup test env..."

BIN_OUT_DIR=out/install/Android/${ARCH}/bin
LIBS_OUT_DIR=out/install/Android/${ARCH}/lib

echo "========================================"
echo "Send files to device..."

if [[ ${usb_state} == 1 ]];then
    adb shell rm -rf /data/local/tmp/$user
    adb shell mkdir -p /data/local/tmp/$user
    adb shell mkdir -p /data/local/tmp/$user/output
    if [[ -e ${BIN_OUT_DIR} ]]; then
        adb push ${BIN_OUT_DIR}     /data/local/tmp/$user
    fi
    if [[ -e ${LIBS_OUT_DIR} ]]; then
        adb push ${LIBS_OUT_DIR}    /data/local/tmp/$user
    fi
    if [[ -e ${DATA_OUT_DIR} ]]; then
        adb push ${DATA_OUT_DIR}    /data/local/tmp/$user
    fi
else
    $SSH $HOST mkdir -pv $user
    $SSH $HOST mkdir -pv $user/bin
    $SSH $HOST mkdir -pv $user/lib
    $SSH $HOST mkdir -pv $user/input
    $SSH $HOST mkdir -pv $user/output

    if [[ -e ${BIN_OUT_DIR} ]]; then
        RSYNC ${BIN_OUT_DIR}/       $HOST:$user/bin
    fi
    if [[ -e ${LIBS_OUT_DIR} ]]; then
        RSYNC ${LIBS_OUT_DIR}/      $HOST:$user/lib
    fi
    if [[ -e ${DATA_OUT_DIR} ]]; then
        RSYNC_DEL ${DATA_OUT_DIR}/  $HOST:$user/input
    fi
fi


echo "========================================"
echo "Run test on device..."
if [[ ${usb_state} == 1 ]]; then
    adb exec-out "cd /data/local/tmp/$user; \
        unset LD_PRELOAD ;\
        export LD_LIBRARY_PATH=./lib; \
        chmod -R 777 ./bin; \
        ./bin/oprtest --benchmark_format=console --benchmark_out_format=csv --benchmark_out=./output/bm_res.csv;"
else
    $SSH -t $HOST "cd $user; \
        unset LD_PRELOAD ;\
        export LD_LIBRARY_PATH=./lib; \
        chmod -R 777 ./bin; \
        ./bin/oprtest --benchmark_format=console --benchmark_out_format=csv --benchmark_out=./output/bm_res.csv;"
fi

echo "========================================"
echo "Fetch output files to host..."
if [[ ${usb_state} == 1 ]];then
    adb pull /data/local/tmp/$user/output ./
else
    RSYNC $HOST:$user/output/ output
fi
