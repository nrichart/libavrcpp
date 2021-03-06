#!/bin/bash

NM=$1
OBJ=$2
OUTPUT=$3
LIBAVR_DIR=$4

missing_symbols=$(${NM} -Cu ${OBJ} | awk '{ print $2; }' | cut -d '<' -f 1 | sort -u)

echo '// generated by nm.sh' > ${OUTPUT}

declare -A symbols
symbols=(
  ["DateTime::dayStr"]="${LIBAVR_DIR}/src/components/rtc/rtc.cc"
  ["twi::TWIInterface"]="${LIBAVR_DIR}/src/comm/twi/twi_hw_interface.cc"
  ["serial::SerialUSARTInterface"]="${LIBAVR_DIR}/src/comm/serial/serial_usart_interface.cc"
  ["serial::sw_serial_delay_table"]="${LIBAVR_DIR}/src/comm/serial/serial_software_interface.cc"
)

for _missing in ${missing_symbols}
do
  echo ${_missing}
  file=${symbols[${_missing}]}
  if [ "x${file}" != "x" ]; then
    echo "#include \"${file}\"" >> ${OUTPUT}
  fi
done
