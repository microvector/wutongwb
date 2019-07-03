LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := findtargetdevice.cpp

LOCAL_MODULE := finder

include $(BUILD_EXECUTABLE)
