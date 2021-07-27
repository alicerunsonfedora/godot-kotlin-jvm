plugins {
    kotlin("jvm") version "1.5.10"
    id("com.utopia-rise.godot-kotlin-jvm")
}

repositories {
    mavenCentral()
}

godot {
    //uncomment to test graal vm native image
//    isGraalExportEnabled.set(true)
//    nativeImageToolPath.set("${System.getenv("GRAALVM_HOME")}/bin/native-image")
//    windowsDeveloperVCVarsPath.set(System.getenv("VC_VARS_PATH"))
}
