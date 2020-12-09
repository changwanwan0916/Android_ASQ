//
// Created by Brisonus-DELL on 2020/11/30.
//

#ifndef SHARED_WAVESYN_H
#define SHARED_WAVESYN_H


#include <iostream>
#include <cmath>

constexpr uint8_t Asq_ChNum = 5;
constexpr float Asq_SampleTime1 = 0.0000208333333;
constexpr uint16_t Asq_AudioLen = 11500;
constexpr uint32_t Asq_SampleRate = 48000;
constexpr uint32_t Asq_IntSampleRate = 24000;
constexpr uint16_t Asq_SampleRatio = Asq_SampleRate / Asq_IntSampleRate;

class WaveSynAlgo
{
public:
    float PrChOutput(uint16_t WaveCANSped, float ChInput, uint16_t Ch_Num)
    {
        GetWaveSynIndex(WaveSynIndex1, WaveSynIndex2, DeltaValue, Ch_Num, WaveCANSped, ChInput);

/*        GetChOutput = (AudioData[4 * 11500 + WaveSynIndex2[Ch_Num]] - AudioData[4 * 11500 + WaveSynIndex1[Ch_Num]]) *
                      DeltaValue[Ch_Num] + AudioData[4 * 11500 + WaveSynIndex1[Ch_Num]];*/

        GetChOutput = (AudioData[Ch_Num * 11500 + WaveSynIndex2[Ch_Num]] - AudioData[Ch_Num * 11500 + WaveSynIndex1[Ch_Num]]) *
                      DeltaValue[Ch_Num] + AudioData[Ch_Num * 11500 + WaveSynIndex1[Ch_Num]];

        return GetChOutput;
    }

    void GetWaveSynIndex(uint16_t *AudioIndex1, uint16_t *AudioIndex2,float *DeltaOutput, uint16_t Ch_No, uint16_t WaveSped, float ChData)
    {


        static float WaveSynCurrTime[Asq_ChNum];
        static float WaveSynSumCurrTime[Asq_ChNum];

        PrSpedCoeff(SpedCoeff, WaveSped, ChData, Ch_No);

        WaveSynTotalTime = (Asq_AudioLen - 1) * Asq_SampleTime1 * Asq_SampleRatio;
        WaveSynCurrTime[Ch_No] = SpedCoeff[Ch_No] * Asq_SampleTime1;
        WaveSynSumCurrTime[Ch_No] += WaveSynCurrTime[Ch_No];

        AudioIndex1[Ch_No] = floor (WaveSynSumCurrTime[Ch_No] * Asq_IntSampleRate );

        DeltaOutput[Ch_No] = WaveSynSumCurrTime[Ch_No] * Asq_IntSampleRate  - AudioIndex1[Ch_No];

        if(AudioIndex1[Ch_No] < (Asq_AudioLen - 1)){

            AudioIndex2[Ch_No] = AudioIndex1[Ch_No] + 1;
        }

        else if(AudioIndex1[Ch_No] == (Asq_AudioLen - 1)){

            AudioIndex2[Ch_No] = 0;
        }
        else{

            AudioIndex1[Ch_No] = AudioIndex1[Ch_No] - Asq_AudioLen;
            AudioIndex2[Ch_No] = AudioIndex1[Ch_No] + 1;

            WaveSynSumCurrTime[Ch_No] = WaveSynSumCurrTime[Ch_No] - WaveSynTotalTime;
        }


    }

    void PrSpedCoeff(float *GetSpedCoeff, uint16_t WaveSpedInput,float ChDataInput, uint8_t GetCh_Num)
    {

        GetSpedCoeff[GetCh_Num] = WaveSpedInput * ChDataInput;

    }



private:

    int16_t AudioData[Asq_ChNum * Asq_AudioLen] = {
    #include"ResAu.dat"
    };

    uint16_t WaveSynIndex1[Asq_ChNum];
    uint16_t WaveSynIndex2[Asq_ChNum];
    float DeltaValue[Asq_ChNum];
    float GetChOutput;

    float WaveSynTotalTime;

    float SpedCoeff[Asq_ChNum];

};

/*#include <cstdint>
#include <atomic>
#include <math.h>
#include <memory>

constexpr uint16_t ASQ_CHNUM = 5;
constexpr uint16_t ASQ_AUDIOLEN = 11500;
constexpr float ASQ_SAMPLETIME = 0.0000208333333;
constexpr float ASQ_SAMPLERATE = 48000;
constexpr float ASQ_INTSAMPLERA = 24000;
constexpr uint16_t ASQ_SAMPLERATERATIO = ASQ_SAMPLERATE/ASQ_INTSAMPLERA;


class WaveSynAlgo
{
public:
    void GetWaveSynData(float *WaveSynOutput1,uint16_t CANSped, uint16_t FrameNum)
    {
        float SumChOutput;

        for (uint16_t i = 0; i <  FrameNum ; i++)
        {
            SumChOutput = 0;

            for(uint16_t j = 0; j < ASQ_CHNUM; j++)
            {

                SumChOutput += WaveSynData(CANSped, ChData[j], j);

            }

            WaveSynOutput1[i] = 0.000003 * SumChOutput;
        }
    }

    float WaveSynData(uint16_t InputCANSpedData, float InputChData, uint8_t Ch_Num)
    {

        PrWaveSynIndex(WaveSynIndex1, WaveSynIndex2, DeltaValue, Ch_Num, InputCANSpedData, InputChData);

        WaveSynOutput = (AudioData[Ch_Num * 11500 + WaveSynIndex2[Ch_Num]] - AudioData[Ch_Num * 11500+ WaveSynIndex1[Ch_Num]]) *
                        DeltaValue[Ch_Num] + AudioData[Ch_Num * 11500+ WaveSynIndex1[Ch_Num]];


        return  WaveSynOutput;


    }

    void PrWaveSynIndex(uint16_t *AudioIndex1, uint16_t *AudioIndex2,float *DeltaOutput,uint8_t Ch_Num, uint16_t InputCANSpedData1, float InputChData1)
    {

        float SpedCoeff[ASQ_CHNUM];

        PrSpedCoeff(SpedCoeff, InputCANSpedData1, InputChData1, Ch_Num);

        float WaveSynTotalTime;
        static float WaveSynCurrTime[ASQ_CHNUM];
        static float WaveSynSumCurrTime[ASQ_CHNUM];

        WaveSynTotalTime = (ASQ_AUDIOLEN - 1) * ASQ_SAMPLETIME * ASQ_SAMPLERATERATIO;
        WaveSynCurrTime[Ch_Num] = SpedCoeff[Ch_Num] * ASQ_SAMPLETIME;
        WaveSynSumCurrTime[Ch_Num] += WaveSynCurrTime[Ch_Num];

        AudioIndex1[Ch_Num] = floor (WaveSynSumCurrTime[Ch_Num] * ASQ_INTSAMPLERA );

        DeltaOutput[Ch_Num] = WaveSynSumCurrTime[Ch_Num] * ASQ_INTSAMPLERA  - AudioIndex1[Ch_Num];

        if(AudioIndex1[Ch_Num] < (ASQ_AUDIOLEN - 1)){

            AudioIndex2[Ch_Num] = AudioIndex1[Ch_Num] + 1;
        }

        else if(AudioIndex1[Ch_Num] == (ASQ_AUDIOLEN - 1)){

            AudioIndex2[Ch_Num] = 0;
        }
        else{

            AudioIndex1[Ch_Num] = AudioIndex1[Ch_Num] - ASQ_AUDIOLEN;
            AudioIndex2[Ch_Num] = AudioIndex1[Ch_Num] + 1;

            WaveSynSumCurrTime[Ch_Num] = WaveSynSumCurrTime[Ch_Num] - WaveSynTotalTime;
        }

    }

    void PrSpedCoeff(float *GetSpedCoeff, uint16_t InputSpedData,float GetChData, uint8_t GetCh_Num)
    {

        GetSpedCoeff[GetCh_Num] = InputSpedData * GetChData;

    }

public:
    float ChData[ASQ_CHNUM] = {0.0001,0.0001,0.0001,0.0001,0.0001};

    int16_t AudioData[ASQ_CHNUM  * ASQ_AUDIOLEN] = {
#include"ResAu.dat"
    };

    uint16_t WaveSynIndex1[ASQ_CHNUM];
    uint16_t WaveSynIndex2[ASQ_CHNUM];
    float DeltaValue[ASQ_CHNUM];

    float WaveSynOutput;

};*/

#endif //SHARED_WAVESYN_H
