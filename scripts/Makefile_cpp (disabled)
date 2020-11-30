# This Makefile is provided as an example of how to build a process that can
# communicate with a Storyboard application.  It may require modifications in
# order to build on your host system.  Please see ReadMe.html for a complete
# explanation

#execute following command first:
# source /opt/st/stm32mp1/2.6-openstlinux-4.19-thud-mp1-19-10-09/environment-setup-cortexa7t2hf-neon-vfpv4-openstlinux_weston-linux-gnueabi

SB_ROOT=/mnt/c/Program\ Files/Crank_Software/Storyboard_Engine/6.0.201905090125/linux-stmA5-armle-opengles_2.0-wayland-obj
SB_INC=-I$(SB_ROOT)/include
SB_LIB=-L$(SB_ROOT)/lib -lgreio

# Windows user add this
#CFLAGS+= -DGRE_TARGET_OS_win32

# Linux users add this 
#CFLAGS+= -mbe32 -DGRE_TARGET_OS_linux
SB_LIB+=-lpthread

all: cluster_update

cluster_update: main.cpp 
	$(CC) $(CFLAGS) $(SB_INC) -o $@ $^ $(SB_LIB)
#	$(CC) $(CFLAGS) $(SB_INC) -o cluster_update main.cpp $(SB_LIB)
cluster_update: Send_Receive_Channel.cpp 
	$(CC) $(CFLAGS) $(SB_INC) -o $@ $^ $(SB_LIB)
#	$(CC) $(CFLAGS) $(SB_INC) -o Send_Receive_Channel Send_Receive_Channel.cpp $(SB_LIB)
#for windows
#	cc $(SB_INC) $(SB_INC) main.cpp -o cluster_update $(SB_LIB)

