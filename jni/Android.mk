LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := ir_cab
LOCAL_SRC_FILES := ir-cab-sim.c
LOCAL_LDFLAGS := -llog -lm
include $(BUILD_SHARED_LIBRARY) 
