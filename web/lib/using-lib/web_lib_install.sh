#!/bin/bash
#ROOT_DIR=`pwd`
#CGIC_LIB=${ROOT_DIR}"/cgic-lib/lib"
#CGIC_INC=${ROOT_DIR}"/cgic-lib/include"
#CGICC_LIB=${ROOT_DIR}"/cgicc-lib/lib"
#CGICC_INC=${ROOT_DIR}"/cgicc-lib/include"
#JSONC_LIB=${ROOT_DIR}"/jsonc-lib/lib"
#JSONC_INC=${ROOT_DIR}"/jsonc-lib/include"
#JSONCCP_LIB=${ROOT_DIR}"/jsoncpp-lib/lib"
#JSONCCP_INC=${ROOT_DIR}"/jsoncpp-lib/include"
#SQLITE_LIB=${ROOT_DIR}"/sqlite-lib/lib"
#SQLITE_INC=${ROOT_DIR}"/sqlite-lib/include"
#SQLITE3=${ROOT_DIR}"/sqlite-lib/bin"
#NEW_PATH_1='export PATH="$PATH:'${ROOT_DIR}'"'
#NEW_PATH_2='export PATH="$PATH:'${SQLITE3}'"'
#echo "${NEW_PATH_1}"
#echo "${NEW_PATH_2}"
#ETC_PROFILE='/etc/profile'
#sudo echo "${NEW_PATH_1}" >> ${ETC_PROFILE}
#sudo echo "${NEW_PATH_2}" >> ${ETC_PROFILE}
#source ${ETC_PROFILE}
chmod 755 ./include/*
chmod 755 ./lib/*
chmod 755 ./bin/*
cp ./include/* /usr/include/ -a
cp ./lib/* /usr/lib/ -a
cp ./bin/* /bin/ -a
cp ./share/sqlite3.1.gz /usr/share/man/man1/ -a
echo "/usr/lib" >> /etc/ld.so.conf
/sbin/ldconfig -v

