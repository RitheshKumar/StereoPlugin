//
//  Mono2Stereo.cpp
//  Upmixer
//
//  Created by Liang on 4/27/16.
//
//

#include "Mono2Stereo.h"


Error_t Mono2Stereo::createInstance(Mono2Stereo*& pMono2Stereo) {
    pMono2Stereo = new Mono2Stereo();
    if (!pMono2Stereo) {
        return kUnknownError;
    }

    return kNoError;
}

Error_t Mono2Stereo::destroyInstance(Mono2Stereo*& pMono2Stereo) {
    if (!pMono2Stereo) {
        return kUnknownError;
    }
    
    pMono2Stereo -> resetInstance();
    
    delete pMono2Stereo;
    pMono2Stereo = 0;
    return kNoError;
}

Error_t Mono2Stereo::initInstance(float sampleRate) {
    m_fSampleRate = sampleRate;
    
    //set parameter ranges
    for (int i = 1; i < kNumOfFilter; i++) {
        m_aaafParamRange[i][kBandWidth][0]  = 100.f;
        m_aaafParamRange[i][kBandWidth][1]  = 800.f;
        m_aaafParamRange[i][kCenterFreq][0] = 100.f;
        m_aaafParamRange[i][kCenterFreq][1] = 20000.f;
        m_aaafParamRange[i][kFilterGain][0] = .01f;
        m_aaafParamRange[i][kFilterGain][1] = 2.f;
    }
    
    //create array of pointers: point to band pass filters
    createFilter();
    
    //set initial parameters for each filter
    
    
    m_bisInitialized = true;
    return kNoError;
}

Error_t Mono2Stereo::resetInstance() {
    //set params to 0
    for (int i = 1; i < kNumOfFilter; i++) {
        for (int j = 0; j < kNumOfParams; i++) {
            setParam((Mono2StereoParam_t)j, 0.f);
        }
    }
    
    delete m_pLeft1Filter;
    m_pLeft1Filter = 0;
    delete m_pLeft2Filter;
    m_pLeft2Filter = 0;
    delete m_pLeft3Filter;
    m_pLeft3Filter = 0;
    delete m_pRight1Filter;
    m_pRight1Filter = 0;
    delete m_pRight2Filter;
    m_pRight2Filter = 0;
    delete m_pRight3Filter;
    m_pRight3Filter = 0;
    
    return kNoError;
}

Error_t Mono2Stereo::setParam(Mono2StereoParam_t param, float paramValue) {
    if (!m_bisInitialized) {
        return kNotInitializedError;
    }
    if (!isInParamRange(param, paramValue)) {
        return kFunctionInvalidArgsError;
    }
    
    return kNoError;
}

Error_t Mono2Stereo::createFilter() {
    //create array of pointers: point to band pass filters
    m_pLeft1Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::LowPass <4>, 1, Dsp::DirectFormII>(1024);
    m_pLeft2Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <4>, 1, Dsp::DirectFormII>(1024);
    m_pLeft3Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <4>, 1, Dsp::DirectFormII>(1024);
    m_pRight1Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <4>, 1, Dsp::DirectFormII>(1024);
    m_pRight2Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <4>, 1, Dsp::DirectFormII>(1024);
    m_pRight3Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::HighPass <4>, 1, Dsp::DirectFormII>(1024);
    
    return kNoError;
}

Error_t Mono2Stereo::initialBandPassFilterParam(std::string filterID, float sampleRate) {
    if (!m_bisInitialized) {
        return kNotInitializedError;
    }
    m_FilterParams[0] = sampleRate;
    if (filterID.compare("Left2")) {
       
    } else if (filterID.compare("Left3")) {
        
    } else if (filterID.compare("Right1")) {
        
    } else if (filterID.compare("Right2")) {
        
    }
    
    
    
    return kNoError;
}