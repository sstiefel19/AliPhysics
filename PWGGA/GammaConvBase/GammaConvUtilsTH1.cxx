#include "GammaConvUtilsTH1.h"
#include "TF1.h"
#include "TH1.h"

//_________________________________________________________________________________________________
/* static */
TF1 *utils_TH1::GlobalPieceWiseExponentialInterpolationTF1(std::string const &theNewName, 
                                                           TH1         const &theTH1, 
                                                           bool               theIntegrate /* = false*/,
                                                           bool               theUseXtimesExp /* = false*/) // fits a function of the f(x) = x * exp([0] + [1]*x)
{
    printf("utils_TH1::GlobalPieceWiseExponentialInterpolationTF1(): called with theNewName: %s, theTH1: %s, theIntegrate = %d, theUseXtimesExp = %d\n",
           theNewName.data(), theTH1.GetName(), theIntegrate, theUseXtimesExp);
    
    utils_TH1::TH1_ExponentialInterpolation_static lInstance(theNewName, 
                                                             theTH1, 
                                                             theIntegrate, 
                                                             theUseXtimesExp);
    
    if (!lInstance.IsInitialized()){
        printf("FATAL: utils_TH1::GlobalPieceWiseExponentialInterpolationTF1():\n"
               "Could not initialize TH1_ExponentialInterpolation object %s with provided"
               " parameters. Returning nullptr.\n",
               lInstance.GetId().data());
               return nullptr;
    }

    TF1 *lResult = lInstance.GetInterpolationTF1(theTH1, theIntegrate, theUseXtimesExp);

    printf("utils_TH1::GlobalPieceWiseExponentialInterpolationTF1(): %sturning %s%s %s.\n",
           lResult ? "Re" : "Not re",
           lResult ? lResult->GetName() : "TF1* for histo",
           lResult ? "." : theTH1.GetName(),
           lResult ? "" : ". Returning nullptr.\n");
    return lResult;
}
