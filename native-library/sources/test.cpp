#include <pthread.h>

#include "Test.h" // Will be generated from the Test.java file, as part of the build process

static JavaVM* javaVm;
static jint jniVersion;
static pthread_key_t vmAttachedEnvironment;
static pthread_once_t vmAttachedEnvironment_key_once = PTHREAD_ONCE_INIT;

static void detachFromVm(void* currentlyAttached) {
	printf("Detaching thread from Java VM...\n"); fflush(stdout);
	if (javaVm->DetachCurrentThread() != 0) {
		printf("Error encountered while detaching thread from Java VM...\n"); fflush(stdout);
	} else {
		printf("Thread detached from Java VM.\n"); fflush(stdout);
	}
}

static void makevmAttachedEnvironmentKey() {
	pthread_key_create(&vmAttachedEnvironment, detachFromVm);
}

static jobject testFunction;
static jclass testFunctionClass;
static jmethodID callMe;

void *callClojure(void *ptr) {
	JNIEnv *env = NULL;
	jint result = javaVm->GetEnv((void**)&env, jniVersion);

	if (result == JNI_EDETACHED) {
		printf("Attaching thread to Java VM...\n"); fflush(stdout);
		if (javaVm->AttachCurrentThread((void **) &env, NULL) != 0) {
			printf("Failed to attach to thread\n"); fflush(stdout);
			return NULL;
		} else {
			printf("Thread attached.\n"); fflush(stdout);
		}
		pthread_setspecific(vmAttachedEnvironment, (void*)env);
	} else if (!env) {
		printf("There's a problem getting the JNI env.\n"); fflush(stdout);
		return NULL;
	} else {
		printf("Thread already attached to JavaVM.\n"); fflush(stdout);
	}

	printf("Calling Clojure function...\n"); fflush(stdout);
	env->CallObjectMethod(testFunctionClass, callMe);
	printf("Clojure call completed.\n"); fflush(stdout);

	return NULL;
}

JNIEXPORT void JNICALL Java_Test_test(JNIEnv *env, jclass obj, jobject testFunctionLocal) {
	if (!javaVm) {
		env->GetJavaVM(&javaVm);
		jniVersion = env->GetVersion();
	}

	pthread_once(&vmAttachedEnvironment_key_once, makevmAttachedEnvironmentKey);

	testFunction = (jobject)env->NewGlobalRef(testFunctionLocal);
	testFunctionClass = (jclass)env->NewGlobalRef(env->GetObjectClass(testFunction));
	callMe = env->GetMethodID(testFunctionClass, "callMe", "()V");

	if (true) {
		printf("Creating thread to call from...\n"); fflush(stdout);
		pthread_t thread;
		pthread_create(&thread, NULL, callClojure, NULL);
		if (pthread_join(thread, NULL) != 0) {
			printf("Error joining thread\n"); fflush(stdout);
		}
	} else {
		printf("Preparing to call from same thread...\n"); fflush(stdout);
		callClojure(NULL);
	}
}
