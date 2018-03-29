/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ptc_jni_Clear */

#ifndef _Included_ptc_jni_Clear
#define _Included_ptc_jni_Clear
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ptc_jni_Clear
 * Method:    create
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_ptc_jni_Clear_create
  (JNIEnv *, jobject);

/*
 * Class:     ptc_jni_Clear
 * Method:    destroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_ptc_jni_Clear_destroy
  (JNIEnv *, jobject, jlong);

/*
 * Class:     ptc_jni_Clear
 * Method:    request
 * Signature: (JLptc/Format;)V
 */
JNIEXPORT void JNICALL Java_ptc_jni_Clear_request
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     ptc_jni_Clear
 * Method:    clear
 * Signature: (JLjava/lang/Object;IIIIILptc/Color;)V
 */
JNIEXPORT void JNICALL Java_ptc_jni_Clear_clear
  (JNIEnv *, jobject, jlong, jobject, jint, jint, jint, jint, jint, jobject);

#ifdef __cplusplus
}
#endif
#endif