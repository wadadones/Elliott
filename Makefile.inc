mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Add o.file written in C
APPL_COBJS += \
	balancer.o \
	balancer_param.o \

# Add o.file written in C++
APPL_CXXOBJS += \
	BalancerCPP.o \
	Operator.o \
	EV3.o \
	Schedule.o \
	Phase.o \
	LineTracer.o \

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

# Add directory has .c or .cpp files
APPL_DIR += \
	 $(mkfile_path)Balancer \
	 $(mkfile_path)Operator \
	 $(mkfile_path)EV3 \
	 $(mkfile_path)Schedule \
	 $(mkfile_path)Phase \
	 $(mkfile_path)Phase/LineTracer \

# Add directory has .h or .hpp files
INCLUDES += \
	-I$(mkfile_path)Balancer \
	-I$(mkfile_path)Operator \
	-I$(mkfile_path)EV3 \
	-I$(mkfile_path)Schedule \
	-I$(mkfile_path)Phase \
	-I$(mkfile_path)Phase/LineTracer \
