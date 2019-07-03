LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := hardkeyevent.cpp

LOCAL_MODULE := keyevent

include $(BUILD_EXECUTABLE)                            
