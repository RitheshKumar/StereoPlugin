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

Error_t Mono2Stereo::initInstance(float sampleRate, int numOfFrames) {
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
    //Left channel 1
    initialBandPassFilterParam("Left1");
    m_pLeft1Filter->setParams(m_FilterParams);
    //Left channel 2
    initialBandPassFilterParam("Left2");
    m_pLeft2Filter->setParams(m_FilterParams);
    //Left channel 3
    initialBandPassFilterParam("Left3");
    m_pLeft3Filter->setParams(m_FilterParams);
    //Right channel 1
    initialBandPassFilterParam("Right1");
    m_pRight1Filter->setParams(m_FilterParams);
    //Right channel 2
    initialBandPassFilterParam("Right2");
    m_pRight2Filter->setParams(m_FilterParams);
    //Right channel 3
    initialBandPassFilterParam("Right3");
    m_pRight3Filter->setParams(m_FilterParams);
    //Common filter
    initialBandPassFilterParam("Common");
    m_pBothChannelFilter->setParams(m_FilterParams);
    
    //allocate memory for temporary buffer based on number of frames per block
    m_pfTempBuffer = new float[numOfFrames * 4];
    
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
    delete m_pBothChannelFilter;
    m_pBothChannelFilter = 0;
    delete m_pfTempBuffer;
    m_pfTempBuffer = 0;
    
    
    
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
    m_pLeft1Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <6>, 1, Dsp::DirectFormII>(1024);
    m_pLeft2Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <6>, 1, Dsp::DirectFormII>(1024);
    m_pLeft3Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <6>, 1, Dsp::DirectFormII>(1024);
    m_pRight1Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <6>, 1, Dsp::DirectFormII>(1024);
    m_pRight2Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <6>, 1, Dsp::DirectFormII>(1024);
    m_pRight3Filter = new Dsp::SmoothedFilterDesign <Dsp::Elliptic::Design::BandPass <6>, 1, Dsp::DirectFormII>(1024);
    
    return kNoError;
}

Error_t Mono2Stereo::initialBandPassFilterParam(std::string filterID) {
    if (!m_bisInitialized) {
        return kNotInitializedError;
    }
    m_FilterParams[0] = m_fSampleRate;
    m_FilterParams[1] = 6;  //order of the filter
    if (filterID.compare("Left1")) {
        m_FilterParams[2] = 3650;   //ceter frequency
        m_FilterParams[3] = 3300;   //bandwidth
    } else if (filterID.compare("Left2")) {
        m_FilterParams[2] = 10250;   //ceter frequency
        m_FilterParams[3] = 3300;   //bandwidth
    } else if (filterID.compare("Left3")) {
        m_FilterParams[2] = 16850;   //ceter frequency
        m_FilterParams[3] = 3300;   //bandwidth
    } else if (filterID.compare("Right1")) {
        m_FilterParams[2] = 6950;   //ceter frequency
        m_FilterParams[3] = 3300;   //bandwidth
    } else if (filterID.compare("Right2")) {
        m_FilterParams[2] = 13550;   //ceter frequency
        m_FilterParams[3] = 3300;   //bandwidth
    } else if (filterID.compare("Right3")) {
        m_FilterParams[2] = 20250;   //ceter frequency
        m_FilterParams[3] = 3500;   //bandwidth
    } else if (filterID.compare("Common")) {
        m_FilterParams[2] = 1025;   //center frequency
        m_FilterParams[3] = 1950;   //band width
    }
    
    
    
    return kNoError;
}

Error_t Mono2Stereo::process(float *pfInputBuffer, float *pfOutputBuffer, int iNumberOfFrames) {
    //1, copy the input buffer to the temp buffer
    memcpy(m_pfTempBuffer, pfInputBuffer, iNumberOfFrames);
    //2, process the temp buffer by a bandpass filter
    m_pLeft1Filter->process(iNumberOfFrames, &m_pfTempBuffer);
    //3, apply filter gain
    
    //4, copy the temp to the output buffer
    memcpy(pfOutputBuffer, m_pfTempBuffer, iNumberOfFrames);
    return kNoError;
}


//////////////////////////////////////////////////////////////////////


//class Mono2StereoTests  : public UnitTest
//{
//public:
//    Mono2StereoTests() : UnitTest ("Mono2Stereo"),
//    _ppfInputData( new float*[3]),
//    _freq(0),
//    _sampleRate(44100),
//    _numSamples(4096),
//    _numChannels(3)
//    {
//        
//        Mono2Stereo::createInstance(testObj);
//        testObj->initInstance(_sampleRate, _numSamples );
//        setVal( _ppfInputData,"init");
//        
//    }
//    
//    ~Mono2StereoTests() { Mono2Stereo::destroyInstance(testObj); }
//    
//    void runTest() override
//    {
//        beginTest ("ZeroCheck"); {
//            
//            std::cout<<"This Test is aashome\n";
//        }
//        
//        
//        beginTest ("DC Check"); {
//            
//            setVal( _ppfInputData, "set", 1.5f );
//            
////            testObj->correlate( (const float**) _ppfInputData, _freq, _numSamples );
//            
////            expect(_freq == 0 );
//        }
//        
//        
//        beginTest("Write Audio Correlated Output"); {
//            
////            FileRW::fileRead( _ppfAudioFile, _numSamples, (char *)"/Users/Rithesh/Documents/Learn C++/ASE/notes/Matlab_ASE/audioIn.txt");
//            
////            testObj -> correlate( (const float**) _ppfAudioFile, _freq, _numSamples);
//            
////            expect(_freq == 441);
//            
//        }
//        
//    }
//private:
//    float **_ppfInputData, _freq, **_ppfAudioFile;
//    int _sampleRate,_numSamples,_numChannels;
//    
//    Mono2Stereo *testObj;
//    
//    //private Function
//    void setVal( float **input, const char *string, float setValue = 0) {
//        
//        
//        if( strcmp("init",string) == 0 ){
//            
//            for( int c = 0; c<_numChannels; c++ ) {
//                _ppfInputData[c] = new float[_numSamples];
//                std::fill(_ppfInputData[c],_ppfInputData[c]+_numSamples, setValue);
//            }
//            
//        }
//        else if( strcmp("set",string) == 0 ){
//            
//            for( int c = 0; c<_numChannels; c++ ) {
//                std::fill(_ppfInputData[c],_ppfInputData[c]+_numSamples, setValue);
//            }
//        }
//    }
//};
//
//static Mono2StereoTests mono2StereoTest;
//
////#endif


