import org.jetbrains.kotlin.gradle.tasks.KotlinCompile
import org.springframework.boot.gradle.tasks.bundling.BootJar

buildscript {
    repositories {
        mavenCentral()
    }
    dependencies {
        classpath(group = "org.springframework.boot", name = "spring-boot-gradle-plugin", version = "2.1.5.RELEASE")
    }
}

plugins {
    java
    kotlin("jvm").version("1.3.31")
    id("org.springframework.boot").version("2.1.5.RELEASE")
    id("io.spring.dependency-management").version("1.0.7.RELEASE")
}

group = "cn.mx404.audiotoass"
version = "0.1.0"


tasks.getByName<BootJar>("bootJar") {
    baseName = "spring-server"
    version = project.version as String
}

repositories {
    mavenCentral()
}

dependencies {
    implementation(kotlin("stdlib-jdk8"))
    implementation(group = "org.springframework.boot", name = "spring-boot-starter-web", version = "2.1.5.RELEASE")
    implementation(project(":SDKWrapServer"))
    testCompile("junit", "junit", "4.12")
}

configure<JavaPluginConvention> {
    sourceCompatibility = JavaVersion.VERSION_1_8
}
tasks.withType<KotlinCompile> {
    kotlinOptions.jvmTarget = "1.8"
}