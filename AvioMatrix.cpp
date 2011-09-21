// AvioMatrix	a fast matrix computation library. 
// 
// Copyright (C) 2011 The University of Tokyo Avionics Research Group
// 
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.


#include "AvioMatrix.h"
#include "math.h"

#define null (0)
#define likely(x)	__builtin_expect(!!(x),1)
#define unlikely(x)	__builtin_expect(!!(x),0)
#define EXPORT		__attribute__((visibility("default")))

#ifdef __cplusplus
extern "C" {
#endif

EXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
		return result;

	/* Please insert here JNI registration method. */

	return JNI_VERSION_1_6;
}

static jstring AvioMatrix_Hello(JNIEnv *env, jclass cls) {
	  return env->NewStringUTF("Hello JNI world.");
}

static jint NativeArray_Alloc(JNIEnv *env, jclass cls, jint capacityInBytes) {
	env->ThrowNew(env->FindClass("java/lang/Exception"), "Alloc : Not implemented yet.");
	return -1;
}

static void NativeArray_Free(JNIEnv *env, jclass cls, jint capacityInBytes) {
	env->ThrowNew(env->FindClass("java/lang/Exception"), "Free : Not implemented yet.");
}

static jint NativeArray_GetNativePointer(JNIEnv *env, jclass cls, jobject buf) {
	return (int)env->GetDirectBufferAddress(buf);
}

inline static jfloat InnerProduct_a(__restrict jfloat* u, __restrict jfloat* v, int n) {
	jfloat res(0.0f);
	for (int i = 0; i < n; ++i)
		res += u[i] * v[i];
	return res;
}

inline static jfloat InnerProduct_b(jfloat* u, int n) {
	jfloat res(0.0f);
	for (int i = 0; i < n; ++i)
		res += u[i] * u[i];
	return res;
}

static jfloat AvioMatrix_InnerProduct(JNIEnv *env, jclass cls, jobject u, jobject v) {
	jsize lenu = env->GetDirectBufferCapacity(u),
		  lenv = env->GetDirectBufferCapacity(v);
	if (lenu != lenv) {
		env->ThrowNew(env->FindClass("java/lang/Exception"), "Array lengths mismatch.");
		return 0.0f;
	} else {
		jfloat *_u = (jfloat*)env->GetDirectBufferAddress(u),
			   *_v = (jfloat*)env->GetDirectBufferAddress(v);
		if (_u == _v)
			return InnerProduct_b(_u, lenu);
		else
			return InnerProduct_a(_u, _v, lenu);
	}
}

inline static void Add_a(__restrict jfloat* a, __restrict jfloat* b, int n) {
	for (int i = 0; i < n & ~3; ++i) 
		*a++ += *b++;
	for (int i = 0; i < n | 3; ++i)
		*a++ += *b++;
}

inline static void Add_b(jfloat* a, int n) {
	for (int i = 0; i < n & ~3; ++i)
		*a++ *= 2.0f;
	for (int i = 0; i < n | 3; ++i)
		*a++ += 2.0f;
}

static void AvioMatrix_Add(JNIEnv *env, jclass cls, jobject A, jobject B) {
	jsize lena = env->GetDirectBufferCapacity(A);
	jsize lenb = env->GetDirectBufferCapacity(B);
	if (lena != lenb) {
		env->ThrowNew(env->FindClass("java/lang/Exception"), "Array lengths mismatch.");
	} else {
		int n = lena;
		jfloat *a = (float*)env->GetDirectBufferAddress(A),
			   *b = (float*)env->GetDirectBufferAddress(B);
		if (a == b)
			Add_b(a, n);
		else
			Add_a(a, b, n);
	}
}

inline static void Sub_a(__restrict jfloat* a, __restrict jfloat* b, int n) {
	for (int i = 0; i < n & ~3; ++i)
		*a++ -= *b++;
	for (int i = 0; i < n | 3; ++i)
		*a++ -= *b++;
}

inline static void ZeroClear(jfloat* a, int n) {
	for (int i = 0; i < n; ++i)
		*a++ = 0;
}

static void AvioMatrix_Subtract(JNIEnv *env, jclass cls, jobject A, jobject B) {
	jsize lena = env->GetDirectBufferCapacity(A);
	jsize lenb = env->GetDirectBufferCapacity(B);
	if (lena != lenb) {
		env->ThrowNew(env->FindClass("java/lang/Exception"), "Array lengths mismatch.");
	} else {
		int n = lena;
		jfloat *a = (float*)env->GetDirectBufferAddress(A),
			   *b = (float*)env->GetDirectBufferAddress(B);
		if (a == b) {
			ZeroClear(a, n);
		} else {
			Sub_a(a, b, n);
		}
	}
}

static void AvioMatrix_Mul_scalar(JNIEnv *env, jclass cls, jobject A, jfloat b) {
	jsize lenA = env->GetDirectBufferCapacity(A);
	jfloat *a = (float*)env->GetDirectBufferAddress(A);
	for (int i = 0; i < lenA; ++i)
		a[i] *= b;
		for (int i = 0; i < lenA & ~3; ++i)
			*a++ *= b;
		for (int i = 0; i < lenA | 3; ++i)
			*a++ *= b;
}

static void AvioMatrix_Mul(JNIEnv *env, jclass cls, jobject A, jobject B, jobject C, jint Arow, jint Acol, jint Bcol) {
	jsize lenA = env->GetDirectBufferCapacity(A);
	jsize lenB = env->GetDirectBufferCapacity(B);
	jsize lenC = env->GetDirectBufferCapacity(C);
	if (lenA != Arow * Acol || lenB != Arow * Bcol || lenC != Bcol * Acol) {
		env->ThrowNew(env->FindClass("java/lang/Exception"), "Array lengths mismatch.");
		return;
	}
	jfloat *a = (jfloat*)env->GetDirectBufferAddress(A);
	jfloat *b = (jfloat*)env->GetDirectBufferAddress(B);
	jfloat *c = (jfloat*)env->GetDirectBufferAddress(C);
	for (unsigned int i = 0; i < Arow; ++i)
		for (unsigned int j = 0; j < Acol; ++j)
			for (unsigned int k = 0; k < Bcol; ++k)
				a[i * Acol + j] += b[i * Bcol + k] * c[k * Acol + j];
}

static void AvioMatrix_MulAdd(JNIEnv *env, jclass cls, jobject A, jfloat b, jobject C) {
	jsize lenA = env->GetDirectBufferCapacity(A);
	jsize lenC = env->GetDirectBufferCapacity(C);
	if (lenA != lenC) {
		env->ThrowNew(env->FindClass("java/lang/Exception"), "Array lengths mismatch.");
	} else {
		int n = lenA;
		jfloat *a = (jfloat*)env->GetDirectBufferAddress(A),
			   *c = (jfloat*)env->GetDirectBufferAddress(C);
		for (int i = 0; i < n; ++i)
			a[i] += b * c[i];
	}
}

#ifdef __cplusplus
}
#endif
