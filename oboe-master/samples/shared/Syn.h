//
// Created by Brisonus-DELL on 2020/12/7.
//

#ifndef SHARED_SYN_H
#define SHARED_SYN_H

#include<iostream>
#include<cmath>

#include"ModFit.h"
#include"WaveSyn.h"

constexpr uint8_t Asq_ChNum1 = 4;
constexpr uint8_t Asq_OderNum1 = 5;

class SynAlgo
{
public:
    void GetSynOutput(float *PrSynOutPut, uint16_t CANSped, uint16_t FramSize)
    {
        ModFitAlgo modfit;
        WaveSynAlgo wavesyn;

        for(uint16_t ii = 0; ii < FramSize; ii ++)
        {
            SumOrderOutput = 0;
            SumChOutput = 0;

            for(uint16_t jj =0; jj < Asq_OderNum1; jj++)
            {

                SumOrderOutput += modfit.PrOrderOutput(CANSped, OrderData[jj], jj);

            }
            for(uint16_t jj = 0; jj < Asq_ChNum1; jj++)
            {
                SumChOutput = wavesyn.PrChOutput(CANSped, Chdata[jj], jj);
            }

            PrSynOutPut[ii] = 10 * (0.01 * SumOrderOutput + 0.000003 * SumChOutput);
        }
    }

private:

    float OrderData[Asq_OderNum1] = {1.5, 2.5, 3.5, 4.5, 5.5};
    float Chdata[Asq_ChNum1] = {0.0001,0.0001,0.0001, 0.0001};

    float SumOrderOutput;
    float SumChOutput;
};
#endif //SHARED_SYN_H
