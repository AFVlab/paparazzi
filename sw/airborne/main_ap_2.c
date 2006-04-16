#include "main_ap.h"
#include "init_hw.h"
#include "int.h"
#include "sys_time.h"
#include "led.h"
#include "modem.h"
#include "gps.h"
#include "print.h"

#include "airframe.h"
uint8_t ac_ident = AC_ID;
float desired_roll = 0.;
float desired_pitch = 0.;
float desired_x = 0.;
float desired_y = 0.;
float desired_altitude;
float desired_climb;
uint8_t pprz_mode;
uint8_t vertical_mode;
uint8_t lateral_mode;
uint8_t horizontal_mode;
uint8_t inflight_calib_mode;
uint8_t mcu1_status;
uint8_t ir_estim_mode;
int32_t nav_utm_east0;
int32_t nav_utm_north0;
/* why not uint8_t */
int8_t nav_utm_zone0;
float estimator_phi;
float estimator_psi;
float estimator_theta;
bool_t in_segment;
int16_t segment_x_1;
int16_t segment_y_1;
int16_t segment_x_2;
int16_t segment_y_2;
float energy;
int16_t desired_gaz;
uint8_t vsupply;
uint16_t estimator_flight_time;
bool_t low_battery;
uint16_t block_time;
uint16_t stage_time;
float climb_sum_err;
float climb_pgain;
float course_pgain;
bool_t in_circle;
int16_t circle_x;
int16_t circle_y;
int16_t circle_radius;

#include "ap_downlink.h"


void init_ap( void ) {
  /* if AP is running in a separate MCU */
#ifndef FBW
  hw_init();
  sys_time_init();
#ifdef LED
  led_init();
#endif /* LED */
#ifdef USE_UART0
  uart0_init_tx();
  uart0_init_rx();
  Uart0PrintString("AP Booting $Id$\n");
#endif  /* USE_UART0 */
#endif  /* FBW */

#ifdef MODEM
  modem_init();
#endif /* MODEM */
#ifdef GPS
  gps_init();
  gps_configure();
#endif
#ifdef ADC

#endif /* ADC */

  /* if AP is running in a separate MCU */
#ifndef FBW
 int_enable();
#endif /* FBW */
}

void periodic_task_ap( void ) {
  //  LED_TOGGLE(1);
  //  LED_TOGGLE(2);
}

void event_task_ap( void ) {
#ifdef DOWNLINK
  ap_downlink_periodic_task();
#endif
#ifdef GPS
  if (GpsBuffer()) {
    ReadGpsBuffer();
    if (gps_msg_received) {
      /* parse and use GPS messages */
      parse_gps_msg();
      gps_msg_received = FALSE;
      if (gps_pos_available) {
	LED_TOGGLE(1);
	use_gps_pos();
	gps_pos_available = FALSE;
      }
    }
  }
#endif /* GPS */
}
