LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := projectdrumroll_shared

LOCAL_MODULE_FILENAME := libprojectdrumroll

LOCAL_SRC_FILES := projectdrumroll/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BaseSceneSelector.cpp \
                   ../../Classes/FileOperation.cpp \
                   ../../Classes/GamePiece.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/GameStateMachine.cpp \
                   ../../Classes/Goals.cpp \
                   ../../Classes/Grid.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/HUD.cpp \
                   ../../Classes/InteractionMenu.cpp \
                   ../../Classes/InteractionMenuItem.cpp \
                   ../../Classes/LevelEditorScene.cpp \
                   ../../Classes/LoadData.cpp \
                   ../../Classes/LogoScene.cpp \
                   ../../Classes/ProjectDrumrollGame.cpp \
                   ../../Classes/SceneSelectorStateMachine.cpp \
                   ../../Classes/ScreenHelper.cpp \
                   ../../Classes/SpritHelper.cpp \
                   ../../Classes/Store.cpp \
                   ../../Classes/TestdataSave.cpp \
                   ../../Classes/TitleScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
Users/akinlin/Desktop/TBDEntertainment/Games/external/Box2D

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,external/Box2D)
