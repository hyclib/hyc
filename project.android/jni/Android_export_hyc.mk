LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    		:= hyc
LOCAL_SRC_FILES 		:= libhyc.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../..

include $(PREBUILT_SHARED_LIBRARY)
