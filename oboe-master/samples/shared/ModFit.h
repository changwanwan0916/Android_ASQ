//
// Created by Brisonus-DELL on 2020/11/30.
//

#ifndef SHARED_MODFOT_H
#define SHARED_MODFOT_H

#include<iostream>
#include<cmath>


constexpr uint8_t Asq_OderNum = 5;
constexpr float Asq_pi = 3.1415926535898;
constexpr float Asq_SampleTime =  0.0000208333333;

class ModFitAlgo
{
public:

    float PrOrderOutput(uint16_t ModCANSped, float OrderInput, uint16_t Order_Num)
    {
        static float Fre[Asq_OderNum];
        static float Phase[Asq_OderNum];

        Fre[Order_Num] = ModCANSped * OrderInput / 60;
        Phase[Order_Num] += 2 * Asq_pi *Fre[Order_Num] * Asq_SampleTime;

        if(Phase[Order_Num] >= 2 * Asq_pi)
        {
            Phase[Order_Num] -= 2 * Asq_pi;
        }

        GetOrderOutPut = sinf(Phase[Order_Num]);

        return GetOrderOutPut;

    }
private:

    float GetOrderOutPut;

};

/*#include <cstdint>
#include <atomic>
#include <memory>
#include <iostream>
#include<cmath>


constexpr uint16_t ASQ_ORDERNUM = 5;
constexpr float ASQ_PI = 3.1415926535898;
constexpr float ASQ_SAMPLETIME = 0.0000208333333;


class ModFitAlgo
{

public:

    void GetModFitData(float *GetModFitOutput, uint16_t CANSpeed, uint16_t FrameNum)
    {

        float SumOrderData;

        for (uint16_t i = 0; i <  FrameNum ; i++)
        {

            SumOrderData = 0;
            for(uint16_t j = 0; j < ASQ_ORDERNUM; j++)
            {

                SumOrderData += PrModFitOutput(CANSpeed, OrderData[j], j);

            }

            GetModFitOutput[i] = 0.01 * SumOrderData;
        }

    }

    float PrModFitOutput(uint16_t CANSpedInput, uint16_t OrderInput,  uint16_t Order_Num)
    {


        float  Output;

        static float Fre[ASQ_ORDERNUM];
        static float Phase[ASQ_ORDERNUM];

        Fre[Order_Num] = (CANSpedInput*OrderInput)/60;

        Phase[Order_Num] += 2*ASQ_PI*Fre[Order_Num]*ASQ_SAMPLETIME;


        if(Phase[Order_Num] >= 2*ASQ_PI)
        {
            Phase[Order_Num] -=  2*ASQ_PI;
        }

        Output = sinf(Phase[Order_Num]);

        return Output;
    }

private:

    uint16_t OrderData[5] = {1, 2, 3, 4, 5};

};*/

#endif //SHARED_MODFOT_H
