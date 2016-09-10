# File: Android.mk

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := controller
LOCAL_SRC_FILES := controller_wrap.cpp controller.cpp commbase.cpp commbase_rs232.cpp controller.cpp helper.cpp lodepng.cpp lodepng_util.cpp oem.cpp oemp.cpp pngdetail.cpp utils.cpp

LOCAL_CFLAGS    := -frtti
LOCAL_LDFLAGS := -Wl,--allow-multiple-definition
include $(BUILD_SHARED_LIBRARY)
