#include <am.h>
#include <nemu.h>
#include<stdio.h>
void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint64_t us_buf[2];
  us_buf[0] = inl(RTC_ADDR);
  us_buf[1] = inl(RTC_ADDR+4);
  uptime->us = (us_buf[1] << 32 | us_buf[0]);
  //printf("time:%d\n",uptime->us);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
