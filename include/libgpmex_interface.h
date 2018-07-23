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
