//
//  Mono2Stereo.h
//  Upmixer
//
//  Created by Liang on 4/27/16.
//
//

#ifndef __Upmixer__Mono2Stereo__
#define __Upmixer__Mono2Stereo__

#include <stdio.h>
#include "ErrorDef.h"
#include "DspFilters/Filter.h"
#include "DspFilters/Dsp.h"

class Elliptic;

class Mono2Stereo {
public:
    enum Mono2StereoParam_t {
        kSampleRate,
        kFilterOrder,
        kCenterFreq,
        kBandWidth,
        kFilterQ,
        kPassRipple,
        kFilterGain,
        kNumOfFilter,
        kNumOfParams
    };
    
    static Error_t createInstance (Mono2Stereo*& pMono2Stereo);
    static Error_t destroyInstance (Mono2Stereo*& pMono2Stereo);
    
    Error_t initInstance(float sampleRate);
    Error_t resetInstance();
    
    Error_t setParam(Mono2StereoParam_t param, float paramValue);
    float getParam() const;
    
    Error_t process (const float *ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    Error_t createFilter();
    Error_t initialBandPassFilterParam(std::string filterID, float sampleRate);

    
private:
    Mono2Stereo();
    ~Mono2Stereo();
    
    bool m_bisInParamRange;
    bool m_bisInitialized;
    
    float m_fSampleRate;
    float m_aaafParamRange[kNumOfFilter][kNumOfParams][2];
//    float m_aaFilterParamTable[kNumOfFilter][kNumOfParams];
    
    Dsp::Params m_FilterParams;
    Dsp::Filter* m_pLeft1Filter;
    Dsp::Filter* m_pLeft2Filter;
    Dsp::Filter* m_pLeft3Filter;
    Dsp::Filter* m_pRight1Filter;
    Dsp::Filter* m_pRight2Filter;
    Dsp::Filter* m_pRight3Filter;
    
    
    
    bool isInParamRange (Mono2StereoParam_t param, float paramValue);
};

#endif /* defined(__Upmixer__Mono2Stereo__) */