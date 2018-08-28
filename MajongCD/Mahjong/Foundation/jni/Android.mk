LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Foundation

LOCAL_MODULE_FILENAME := libFoundation

LOCAL_SRC_FILES := ../Classes/Tcp.cpp \
				   ../Classes/Thread.cpp \
				   ../Classes/TcpService.cpp \
				   ../Classes/MutexLock.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)../Classes\


include $(BUILD_STATIC_LIBRARY)
