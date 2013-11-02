all:
	./build_native.sh
	ant -Dsdk.dir=C:\Users\user\android-sdks debug
run:
	adb -d uninstall com.tbdentertainment.projectdrumroll
	adb -d install bin\ProjectDrumroll-debug.apk
	adb -d logcat | grep cocos2d
clean:
	rm -rf libs\
	rm -rf obj\
	rm -rf bin\*
	rm -rf gen\
	rm -rf assets\*