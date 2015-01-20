#!/bin/bash

##########################  FUNCTION UTILITIES #################################

validate_folder()
{
    if [ ! -d "$1" ]; then
    echo "Directory $1 Not found"
    exit 1
    fi
}

exit_cleanup()
{
    rm -rf $LTIB_DEP;
	echo "Exit Build"
	exit;
}

cmd_status()
{
    if [ $? -ne 0 ]; then
		echo "Cmd failed"
	    exit_cleanup;
    fi
}

################################################################################

############################        START SCRIPT      ##########################

echo "Veryfy LTIB_PATH"
validate_folder $LTIB_PATH;

#############################  U-BOOT KERNEL APPS DRV ###########################
LTIB_PKG="/opt/freescale/pkgs/"
LTIB_BLD_SUP="$PWD/ltib_build_support"
LTIB_DEP="$PWD/ltib_dep"
APPS_DIR="$PWD/source/apps"
LIB_DIR="$PWD/source/lib"
#DRV_DIR="$PWD/source/driver"    			#ap
TEST_DIR="$PWD/source/test"
IMG_DIR="$PWD/target/images"
#################################################################################

############################  UBOOT KERNL APPs DRVs   ###########################
UBOOT="u-boot-tdmOip-1.3.3"
KERN="linux-tdmOip-2.6.25"
TEST="test"
APPS="cfg_mgr webs_mgr" # mac_upld_mgr probe_rd_mgr     #ap
#DRV="compat_wl12xx"					#ap
LIB="conn_lib"
VER="1.0"
#################################################################################

###########################  Build Config Path        ############################
LTIB_CFG_DIR="$LTIB_PATH/config/userspace/"
LTIB_PLT_DIR="$LTIB_PATH/config/platform/"
LTIB_SPEC_DIR="$LTIB_PATH/dist/lfs-5.1/"
#################################################################################

###########################     LTIB BUILD DEP      #############################
LTIB_ROOTFS="$LTIB_PATH/rootfs/"
LTIB_USER_CONFIG="$LTIB_BLD_SUP/config/"
LTIB_SPEC_CONFIG="$LTIB_BLD_SUP/spec"
LTIB_CMP_DEP="$PWD/source/common"
#################################################################################

echo "Starting CLTU Build"
###########################           LTIB DEP        ###########################
mkdir -p $LTIB_DEP;             
cmd_status;

echo "Copy the package config to ltib"
cp -f $LTIB_USER_CONFIG/* $LTIB_CFG_DIR/
cmd_status;
##############################################################################
###########################       GEN LIB tar.gz    ###########################
cd $LTIB_DEP
cmd_status;
for word in $LIB
do
    validate_folder $LIB_DIR/$word/;
	cp -rf $LIB_DIR/$word $word-$VER;
    cmd_status;
        cp -f $LTIB_CMP_DEP/*.h $word-$VER/inc/
    cmd_status;
	tar -czf $word-$VER.tar.gz $word-$VER;
    cmd_status;
	rm -rf $word-$VER;
    cmd_status;
	mkdir -p $LTIB_SPEC_DIR/$word;
    cmd_status;
	cp -f $LTIB_SPEC_CONFIG/$word.spec $LTIB_SPEC_DIR/$word/
    cmd_status;
done
cd -;
##############################################################################
###########################       GEN DRV tar.gz    ##########################		#ap
cd $LTIB_DEP
cmd_status;
for word in $DRV
do
    validate_folder $DRV_DIR/$word/;
        cp -rf $DRV_DIR/$word $word-$VER;
    cmd_status;
        tar -czf $word-$VER.tar.gz $word-$VER;
    cmd_status;
        rm -rf $word-$VER;
    cmd_status;
        mkdir -p $LTIB_SPEC_DIR/$word;
    cmd_status;
        cp -f $LTIB_SPEC_CONFIG/$word.spec $LTIB_SPEC_DIR/$word/
    cmd_status;
done
cd -;

###############################################################################
###########################       GEN APPS tar.gz    ###########################
cd $LTIB_DEP
cmd_status;
for word in $APPS
do
    validate_folder $APPS_DIR/$word/;
	cp -rf $APPS_DIR/$word $word-$VER;
    cmd_status;
    if [ $word = "hostapd" ]; then		
  	echo "nothing to copy"
    else
        cp -f $LTIB_CMP_DEP/*.h $word-$VER/inc/
   fi
   cmd_status;
	tar -czf $word-$VER.tar.gz $word-$VER;
    cmd_status;
	rm -rf $word-$VER;
    cmd_status;
	mkdir -p $LTIB_SPEC_DIR/$word;
    cmd_status;
	cp -f $LTIB_SPEC_CONFIG/$word.spec $LTIB_SPEC_DIR/$word/;
    cmd_status;
done
cd -;
###############################################################################
#########################       MOVE DEP TO PKGS   ############################
cd $LTIB_DEP
cmd_status;
cp -fr * $LTIB_PKG;
cmd_status;
cd -;
###############################################################################
###############################################################################
rm -rf $LTIB_DEP;
cmd_status;
echo "CLTU Build Success"
#########################      END SCRIPT          ############################
