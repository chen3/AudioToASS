import com.google.protobuf.gradle.*
import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

buildscript {
    repositories {
        mavenCentral()
    }
    dependencies {
        classpath(group = "com.google.protobuf", name = "protobuf-gradle-plugin", version = "0.8.8")
    }
}

plugins {
    java
    kotlin("jvm") version "1.3.31"
    id("com.google.protobuf").version("0.8.8")
}

protobuf {
    protoc {
        artifact = "com.google.protobuf:protoc:3.7.1"
    }
    plugins {
        id("grpc") {
            artifact = "io.grpc:protoc-gen-grpc-java:1.21.0"
        }
    }
    generateProtoTasks {
        ofSourceSet("main").forEach {
            it.plugins {
                // Apply the "grpc" plugin whose spec is defined above, without options.
                id("grpc")
            }
        }
    }
}

group = "cn.mx404.audiotoass"
version = "0.1.0"

repositories {
    mavenCentral()
}

val grpcVersion = "1.21.0";

dependencies {
    implementation(kotlin("stdlib-jdk8"))
    implementation(group = "io.grpc", name = "grpc-netty-shaded", version = grpcVersion)
    implementation(group = "io.grpc", name = "grpc-protobuf", version = grpcVersion)
    implementation(group = "io.grpc", name = "grpc-stub", version = grpcVersion)
    implementation(group = "org.jetbrains.kotlinx", name = "kotlinx-coroutines-core", version = "1.3.0-M1")
    implementation(group = "org.slf4j", name = "slf4j-api", version = "1.7.26")
    protobuf(files(File(rootProject.rootDir.parent, "Protos/Project.proto").absolutePath))
    if (JavaVersion.current().isJava9Compatible) {
        // Workaround for @javax.annotation.Generated
        // see: https://github.com/grpc/grpc-java/issues/3633
        implementation("javax.annotation:javax.annotation-api:1.3.1")
    }
    testCompile("junit", "junit", "4.12")
}

configure<JavaPluginConvention> {
    sourceCompatibility = JavaVersion.VERSION_1_8
}
tasks.withType<KotlinCompile> {
    kotlinOptions.jvmTarget = "1.8"
}