// AvioMatrix	a fast matrix computation library. 
// 
// Copyright (C) 2011 The University of Tokyo Avionics Research Group
// 
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <jni.h>

/* Header for class avionics_matrix_AvioMatrix */

#ifndef _Included_avionics_matrix_AvioMatrix
#define _Included_avionics_matrix_AvioMatrix
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:		avionics/matrix/NativeArray
 * Method:		Alloc
 * Signature:	(I)I
 */
static jint NativeArray_Alloc(JNIEnv *env, jclass cls, jint capacityInBytes); 

/*
 * Class:		avionics/matrix/NativeArray
 * Method:		Free
 * Signature:	(I)V
 */
static void NativeArray_Free(JNIEnv *env, jclass cls, jint pointer); 

/*
 * Class:		avionics/matrix/NativeArray
 * Method:		GetNativePointer
 * Signature	(Ljava/nio/FloatBuffer;)I
 */
static jint NativeArray_GetNativePointer(JNIEnv *env, jclass cls, jobject buf); 

/*
 * Class:     avionics_matrix_AvioMatrix
 * Method:    InnerProduct
 * Signature: (Ljava/nio/FloatBuffer;Ljava/nio/FloatBuffer;)F
 */
static jfloat AvioMatrix_InnerProduct
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     avionics_matrix_AvioMatrix
 * Method:    Add
 * Signature: (Ljava/nio/FloatBuffer;Ljava/nio/FloatBuffer;)V
 */
static void AvioMatrix_Add
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     avionics_matrix_AvioMatrix
 * Method:    Subtract
 * Signature: (Ljava/nio/FloatBuffer;Ljava/nio/FloatBuffer;)V
 */
static void AvioMatrix_Subtract
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     avionics_matrix_AvioMatrix
 * Method:    Mul
 * Signature: (Ljava/nio/FloatBuffer;F)V
 */
static void AvioMatrix_Mul_scalar
  (JNIEnv *, jclass, jobject, jfloat);

/*
 * Class:     avionics_matrix_AvioMatrix
 * Method:    Mul
 * Signature: (Ljava/nio/FloatBuffer;Ljava/nio/FloatBuffer;Ljava/nio/FloatBuffer;III)V
 */
static void AvioMatrix_Mul
  (JNIEnv *, jclass, jobject, jobject, jobject, jint, jint, jint);

/*
 * Class:     avionics_matrix_AvioMatrix
 * Method:    MulAdd
 * Signature: (Ljava/nio/FloatBuffer;FLjava/nio/FloatBuffer;)V
 */
static void AvioMatrix_MulAdd
  (JNIEnv *, jclass, jobject, jfloat, jobject);

#ifdef __cplusplus
}
#endif
#endif
