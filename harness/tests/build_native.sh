/Library/Java/JavaVirtualMachines/graalvm-ce-java16-21.1.0/Contents/Home/bin/native-image -cp build/libs/godot-bootstrap.jar:build/libs/main.jar --shared -H:Name=usercode -H:JNIConfigurationFiles=graal/jniconfig.json -H:IncludeResources=build/resources/main/META-INF/services/*.* --no-fallback --verbose
mv usercode.dylib build/libs/usercode.dylib
