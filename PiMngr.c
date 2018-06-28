#include <stdint.h>
#include "PiMngr.h"
#include "PiMngr_Cfg.h"
#include "UART.h"
#include "UART_Cfg.h"

PiStates State = Initial;
static uint8_t RxBuff[20];


void PiMngr_GetStatus(void)
{
    if(State == Initial)
    {
        UART_Tx_Init("Get Status", 10, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
}

void PiMngr_StartRecognition(void)
{
    //if(State == ReadytoRecognize)
    //{
        State = ReadytoRecognize;
        UART_Tx_Init("Start", 5, UART_ChannelIdTx);
    //}
    //else
    //{
        /* Do nothing */
    //}
}

void PiMngr_RequestData(void)
{
    if(State == ReadytoReceive)
    {
        UART_Tx_Init("Send", 4, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
    if(State == ReceptionDone)
    {
        UART_Tx_Init("Done", 4, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
}

void PiMngr_TxDone(void)
{
    uint8_t LoopIndex;
    if(State == Initial)
    {
        UART_Rx_Init(RxBuff, 5, UART_ChannelIdRx);
    }
    else if(State == ReadytoRecognize)
    {
        UART_Rx_Init(RxBuff, 4, UART_ChannelIdRx);
    }
    else if(State == ReadytoReceive)
    {
        UART_Rx_Init(RxBuff, 11, UART_ChannelIdRx);
    }
    else if(State == ReceptionDone)
    {
        State = Initial;
        ID = RxBuff[0];
        DataLen = RxBuff[1];
        for(LoopIndex = 0; LoopIndex < DataLen; LoopIndex++)
        {
            Data[LoopIndex] = RxBuff[LoopIndex + 2];
        }
    }
    else
    {
        /* Do nothing */
    }

}

void PiMngr_RxDone(void)
{
    if(State == Initial)
    {
        if((RxBuff[0] == 'R') && (RxBuff[1] == 'e') && (RxBuff[2] == 'a') && (RxBuff[3] == 'd') && (RxBuff[4] == 'y'))
        {
            State = ReadytoRecognize;
        }
        else
        {
            //State = BusytoRecognize;
        }
    }
    else if(State == ReadytoRecognize)
    {
        if((RxBuff[0] == 'D') && (RxBuff[1] == 'o') && (RxBuff[2] == 'n') && (RxBuff[3] == 'e'))
        {
            State = ReadytoReceive;
        }
        else
        {
            //State = BusytoReceive;
        }
    }
    else if(State == ReadytoReceive)
    {
        State = ReceptionDone;
        //UART_Tx_Init("Done", 4, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
}
