#!/bin/bash

basename=$0;
usage() {
    echo "cheetah pipeline network performance throughput test"
    echo "Synopsis:"
    echo "  ${basename} [-s] [-b] [-e remote_emulator_executable] [-l username] <remote_ip>"
    echo "Where:"
    echo "  -b : perform cheetah emulator test on the loopback device"
    echo "  -s : perform an iperf speed profile test over the link (default inactive)"
}

# --------------- Default Settings -------------
# -- remote settngs
REMOTE_IPERF_EXE="iperf"
REMOTE_EMULATOR_EXE=cheetah_emulator
REMOTE_USER=$USER

# -- local settngs
IPERF_EXE=iperf3
GNUPLOT=gnuplot
EMULATOR_EXE="./cheetah/emulator/cheetah_emulator"
SSH_EXE="/usr/bin/ssh"
# -- use the cheetah executables from local build area if they exist
if [ -d ./cheetah/pipeline ]; then
    export PATH=`pwd`/cheetah/pipeline:${PATH}
fi

link_test=0
loopback_test=0
# ------------- End Default Settings -------------

# ============ Command Line Parsing ================
while getopts "bh?sl:e:" opt; do
    case "$opt" in
        h|\?)
            usage
        exit 0
        ;;
        l)  REMOTE_USER=$OPTARG
        ;;
        e)  REMOTE_EMULATOR_EXE=${OPTARG}
        ;;
        s)  link_test=1
        ;;
        b)  loopback_test=1
        ;;
        *)
            echo "unknow option"
        usage
        exit 1;
        ;;
    esac
done
shift $((OPTIND-1))

if [ $# -lt 1 ]; then
    usage
    exit 1;
fi
REMOTE_IP="$1"
shift;

# =================  Internals =========================
set -m   # enable job control so we can run in the background
set -e   # abort if something fails
REMOTE_LOGIN="-l ${REMOTE_USER} ${REMOTE_IP}"
SSH_CONTROL=/tmp/ssh-control-`date +%s`-$RANDOM
REMOTE="${SSH_EXE} -o BatchMode=yes -S ${SSH_CONTROL} ${REMOTE_LOGIN}"

wait_jobs() {
    # Wait for all parallel jobs to finish
    while [ 1 ]; do fg 2> /dev/null; [ $? == 1 ] && break; done
}

kill_jobs() {
    # -- remove the SIGTERM trap handler internally to avoid recursion when we invoke kill
    echo "cleaning up...."
    ${SSH_EXE} -O exit -S ${SSH_CONTROL} ${REMOTE_LOGIN}
    trap - SIGTERM && kill -- -$$
}

remote() {
    ${REMOTE} '"$@"'
}

# -- cleanup however we exit
trap kill_jobs SIGTERM SIGINT EXIT

# ================ End Internals =======================
# -- open connectiooon to the remote
${SSH_EXE} -NfM -S "${SSH_CONTROL}" ${REMOTE_LOGIN}
echo "status: connection to ${REMOTE_IP} established"

# ================== Start Tests =======================
TEST_RESULT_DIR=test_results_`date +%s`
mkdir -p ${TEST_RESULT_DIR}
cd ${TEST_RESULT_DIR}

# -- store information about the machines in the test
ARCH_INFO_FILE="architectures"
echo -n "local  :" > ${ARCH_INFO_FILE}
/bin/uname -mprsv >> ${ARCH_INFO_FILE}
echo -n "remote :" >> ${ARCH_INFO_FILE}
${REMOTE} /bin/uname -a >> ${ARCH_INFO_FILE}
if ${REMOTE} -q "[[ -f ${REMOTE_EMULATOR_EXE} ]]"; then
   echo "remote executable ${REMOTE_EMULATOR_EXE} found"
else
   echo "remote executable ${REMOTE_EMULATOR_EXE} does not exist - please set it with the -e option"
   exit 1;
fi

# -- detrmine local interface used for the connection to remote
LOCAL_IP=`${REMOTE} <<'END_LOCL_IP'
echo $SSH_CLIENT | awk '{ print $1}'
END_LOCL_IP
`
echo "starting test between local ${LOCAL_IP} and remote ${REMOTE_IP} ...."

# ---------------------------
# -- test the link speed
# ---------------------------
if [ $link_test -ne 0 ]; then
    echo "running link tests"
    IPERF_TEST_DATA="iperf_link_test.dat"
    port=345787
    ${IPERF_EXE} -s -p $port &
    IPERF_SERVER_PID=$!
    sb_size=1024
    sb_max=8192

    while [ $sb_size -le $sb_max ]; do
        echo -n "$sb_size ," >> ${IPERF_TEST_DATA}
        ${REMOTE} ${REMOTE_IPERF_EXE} -N -c ${LOCAL_IP} -p $port -u -l $sb_size -w $sb_size -b 50000M >> ${IPERF_TEST_DATA}  || true
        sb_size=$((${sb_size} + 512))
    done
    kill ${IPERF_SERVER_PID}
fi

#${GNUPLOT} << EOF
#set xlabel "socket buffer size"
#set ylabel "GB/s"
#plot $iPERF_TEST_DATA}
#EOF

# ---------------------------
# -- test the emulator speed
# ---------------------------
# -- test emulator performance
# --- local
#echo "running emulator speed tests"
#rem_emulator_rate=`${REMOTE} ${REMOTE_EMULATOR_EXE} --emulator.generator null --speed-test`

# ---------------------------
# -- test the loopback
# ---------------------------
num_threads_max=8
if [ $loopback_test -ne 0 ]; then
echo "running loopback tests"
    LOOPBACK_TEST_OUTPUT="pipeline_timing_test_loopback.dat"
    num_threads=1
    while [ $num_threads -le $num_threads_max ]; do
        echo -n "${num_threads}," >> ${LOOPBACK_TEST_OUTPUT}
        echo cheetah_rcpt_performace_test --emulator.generator null --udp.number_of_threads ${num_threads}
        cheetah_rcpt_performace_test --emulator.generator null --udp.number_of_threads ${num_threads} >> ${LOOPBACK_TEST_OUTPUT} || true
        num_threads=$((${num_threads} + 1))
    done
fi

# ---------------------------
# -- test pipeline over link performance
# ---------------------------
LINK_TEST_OUTPUT="pipeline_timing_test_link.dat"
# -- start the emulator on the remote machine
${REMOTE} ${REMOTE_EMULATOR_EXE} --emulator.generator null --emulator.client.ip_address ${LOCAL_IP} &
if [ $? -eq 0 ]; then
    REMOTE_PID=$!
    num_threads=1
    # -- start the timing pipeline
    echo "running pipeline link tests"
    while [ $num_threads -le $num_threads_max ]; do
        echo -n "${num_threads}," >> ${LINK_TEST_OUTPUT}
        echo cheetah_pipeline -p Empty --empty.exit_count 4000 -s udp --data.source.udp.listen.ip_address 0.0.0.0 --data.source.udp.listen.port 3434 --data.source.udp.number_of_threads ${num_threads}
        cheetah_pipeline -p Empty --empty.exit_count 4000 -s udp --data.source.udp.listen.ip_address 0.0.0.0 --data.source.udp.listen.port 3434 --data.source.udp.number_of_threads ${num_threads} >> ${LINK_TEST_OUTPUT} || true
        num_threads=$((${num_threads} + 1))
    done
    ${REMOTE} kill ${REMOTE_PID}
fi

# ==================== End Tests =======================
