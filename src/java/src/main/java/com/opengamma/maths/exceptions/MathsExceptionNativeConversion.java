/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.exceptions;

/**
 * Provides a manner in which maths exceptions relating to native code errors during the conversion from Java ASTs to C++ ASTs
 */
public class MathsExceptionNativeConversion extends MathsExceptionNative {
  /**
   * This form of the constructor is used on Windows, where no native stack trace is provided.
   * @param msg the error message
   */
  public MathsExceptionNativeConversion(String msg) {
    super(msg);
  }

/**
   * Serial ID
   */
  private static final long serialVersionUID = -303234672217705296L;

  /**
   * This form of the constructor is used by the native code for generating the mixed-language backtrace.
   * Frames are represented by 4 strings, containing the class name, method name, file name and line no.
   * The Java side steps through this array to construct suitable StackTraceElement objects to push on
   * top of the Java stack trace.
   *
   * @param msg The error message as presented to the user
   * @param backtraceInfo strings describing each frame
   */
  public MathsExceptionNativeConversion(String msg, String[] backtraceInfo) {
    super(msg, backtraceInfo);
  }
}
