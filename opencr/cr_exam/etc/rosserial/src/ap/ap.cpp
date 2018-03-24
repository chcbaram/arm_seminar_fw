/*
 * ap.c
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */


#include "ap.h"

#include <ros.h>
#include <std_msgs/Byte.h>


ros::NodeHandle nh;

std_msgs::Byte button_msg;
ros::Publisher pub_button("button", &button_msg);





void updateLed(void);



void apInit(void)
{

  nh.initNode();
  nh.advertise(pub_button);
}

void apMain(void)
{

  while(1)
  {
    updateLed();

    uint8_t reading = 0;
    static uint32_t pre_time;


    if (buttonGetPressed(0) == true)
    {
      reading |= 0x01;
    }
    if (buttonGetPressed(1) == true)
    {
      reading |= 0x02;
    }

    if (millis()-pre_time >= 50)
    {
      button_msg.data = reading;
      pub_button.publish(&button_msg);
      pre_time = millis();
    }

    nh.spinOnce();
  }
}

void updateLed(void)
{
  static uint32_t pre_time = 0;


  if (millis()-pre_time >= 1000)
  {
    pre_time = millis();
    ledToggle(_HW_DEF_LED_BOOT);
  }
}

