// Copyright 2018 Toyota Research Institute.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstring>
#include <vector>

#include <mex.h>

#include <class_handle.h>

// A collection of utility functions for MATLAB's mex types to C++ types.
namespace mex_utils {

// Enumerate two of the accepted input types to the mex call, besides vectors.
enum InputType { SCALAR, STRING };

// Checks whether the input type is a scalar or a string.
static int CheckInputType(const mxArray *in, InputType type) {
  bool ischar = false;
  if (mxIsChar(in))
    ischar = true;

  switch (type) {
  case SCALAR:
    if (!ischar)
      return 0;
    mexWarnMsgTxt("Arg Should be of type: Scalar");
    return -1;
  case STRING:
    if (ischar)
      return 0;
    mexWarnMsgTxt("Arg Should be of type: String");
    return -1;
  default:
    mexWarnMsgTxt("Type not recognized.");
    return -1;
  }

  return -1;
}

// Check whether the input is valid.
static int CheckInput(int nrhs, int expected, const char *fcn) {
  if (nrhs != expected) {
    char warn_msg[128];

    // The implicit inputs to function calls are:
    // prhs = ['functionname', obj_handle, arg1, arg2... etc]
    // so the total number of args should be nrhs-2.
    sprintf(warn_msg, "%s: Number of arg %i is incorrect -- expected %i args",
            fcn, nrhs - 2, expected - 2);
    mexWarnMsgTxt(warn_msg);
    return -1;
  }
  return 0;
}

// Given a MATLAB string in, output a std string.
std::string GetStdString(const mxArray *in) {
  int fcnlen = 128;
  char fcn[fcnlen];
  mxGetString(in, fcn, fcnlen);
  return std::string(fcn);
}

// Convert MATLAB vector 'in' to vector of doubles 'out'.
static int MxArray2Vec(const mxArray *in, std::vector<double> *out) {
  int nrows = mxGetM(in);
  int ncols = mxGetN(in);

  // Check if the input is actually a vector.
  if ((ncols != 1) && (nrows != 1)) {
    char fcn[128];
    sprintf(fcn, "Expected an array (1 x n or n x 1), got %i x %i", nrows,
            ncols);
    mexWarnMsgTxt(fcn);
    return -1;
  }

  unsigned int length = ncols == 1 ? nrows : ncols;

  double *p = mxGetPr(in);

  out->clear();
  std::copy(p, p + length, std::back_inserter(*out));

  return 0;
}

// Convert MATLAB matrix 'in' to vector of doubles 'out'.
static int MxArray2Mat(const mxArray *in, std::vector<double> *out) {
  int nrows = mxGetM(in);
  int ncols = mxGetN(in);

  // Check if the input is actually a matrix.
  if (ncols < 1 || nrows < 1) {
    char warn_msg[128];
    sprintf(warn_msg, "Invalid: Got %i x %i matrix", nrows, ncols);
    mexWarnMsgTxt(warn_msg);
    return -1;
  }

  unsigned int length = ncols * nrows;

  double *p = mxGetPr(in);

  out->clear();
  std::copy(p, p + length, std::back_inserter(*out));

  return 0;
}

// Convert vector of doubles 'v' to a MATLAB vector.
mxArray *Vec2MxArray(const std::vector<double> &v) {
  mxArray *mx = mxCreateDoubleMatrix(1, v.size(), mxREAL);
  std::copy(v.begin(), v.end(), mxGetPr(mx));
  return mx;
}
}
