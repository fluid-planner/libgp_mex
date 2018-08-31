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

#include <libgp_interface.h>
#include <mex.h>

#include <class_handle.h>
#include <mex_utils.h>

// Class LibgpMexInterface creates a mex interface to access the libgp library.
class LibgpMexInterface {
public:
  LibgpMexInterface();
  ~LibgpMexInterface();

  // Return status that helps output useful warning messages.
  enum ReturnStatus {
    SUCCESS,
    EXIT,
    ERROR_INPUT,
    ERROR_FAILED,
    ERROR_UNHANDLED,
    ERROR_MATRIX,
    ERROR_MATRIX_SIZE,
  };

  // Calls the appropriate function given the input.
  ReturnStatus FunctionCall(int nlhs, mxArray *plhs[], int nrhs,
                            const mxArray *prhs[]);

private:
  // Initializes the GP.
  // Corresponds to the MATLAB call: gpmex.initialize(input_dim, cov_string,
  // hyp_params).
  ReturnStatus CallInitialize(int nlhs, mxArray *plhs[], int nrhs,
                              const mxArray *prhs[]);

  // Trains the GP with training data X, Y.
  // Corresponds to the MATLAB call: gpmex.train(X, Y).
  ReturnStatus CallTrain(int nlhs, mxArray *plhs[], int nrhs,
                         const mxArray *prhs[]);

  // Generates predictions for data X with variance.
  // Corresponds to the MATLAB call: [y_pred, y_var] =
  // gpmex.predictWithVariance(X).
  ReturnStatus CallPredictWithVariance(int nlhs, mxArray *plhs[], int nrhs,
                                       const mxArray *prhs[]);

  // Generates predictions given X.
  // Corresponds to the MATLAB call: y_pred = gpmex.predict(X).
  ReturnStatus CallPredict(int nlhs, mxArray *plhs[], int nrhs,
                           const mxArray *prhs[]);

  // Clears the data in the GP.
  // Corresponds to the MATLAB call: gpmex.clear().
  ReturnStatus CallClear(int nlhs, mxArray *plhs[], int nrhs,
                         const mxArray *prhs[]);

  // Get the number of samples in the current GP.
  // Corresponds to the MATLAB call: gpmex.getNumSamples().
  ReturnStatus CallGetNumSamples(int nlhs, mxArray *plhs[], int nrhs,
                                 const mxArray *prhs[]);

  // The libgp interface object maintained by this interface.
  LibgpInterface libgp_;
};
