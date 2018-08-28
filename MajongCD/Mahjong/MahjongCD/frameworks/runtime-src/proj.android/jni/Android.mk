LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Foundation
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libFoundation.a
include $(PREBUILT_STATIC_LIBRARY) 
include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua

LOCAL_SRC_FILES := \
../../Classes/AppDelegate.cpp \
hellolua/main.cpp \
../../Classes/ImageLoader.cpp \
../../Classes/UpdateService.cpp \
../../Classes/CFileUtil.cpp \
../../Classes/ClientRequestDataPackage.cpp \
../../Classes/FileEncryptor.cpp \
../../Classes/sha1.cpp \
../../Classes/UpdateHelper.cpp \
../../Classes/EscapeCode.cpp \
../../../cocos2d-x/cocos/scripting/lua-bindings/auto/lua_imageLoader_auto.cpp \
../../../cocos2d-x/cocos/scripting/lua-bindings/auto/lua_updateService_auto.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
										$(LOCAL_PATH)/../../../cocos2d-x/cocos/scripting/lua-bindings/auto \
										$(LOCAL_PATH)/../../../../../Shared \
										$(LOCAL_PATH)/../../../cocos2d-x/external \
										$(LOCAL_PATH)/../../../../../Foundation/Classes
										
# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += libFoundation
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,scripting/lua-bindings/proj.android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
