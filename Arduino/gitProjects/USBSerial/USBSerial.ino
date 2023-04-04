#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup(){}
void loop(){}
#else
#include "USB.h"
/*
#if ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial0
#define USBSerial Serial
#else
*/
#define HWSerial Serial
USBCDC USBSerial;
//#endif

static void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
  if(event_base == ARDUINO_USB_EVENTS){
    arduino_usb_event_data_t * data = (arduino_usb_event_data_t*)event_data;
    switch (event_id){
      case ARDUINO_USB_STARTED_EVENT:
        HWSerial.println("USB PLUGGED");
        break;
      case ARDUINO_USB_STOPPED_EVENT:
        HWSerial.println("USB UNPLUGGED");
        break;
      case ARDUINO_USB_SUSPEND_EVENT:
        HWSerial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
        break;
      case ARDUINO_USB_RESUME_EVENT:
        HWSerial.println("USB RESUMED");
        break;
      
      default:
        break;
    }
  } else if(event_base == ARDUINO_USB_CDC_EVENTS){
    arduino_usb_cdc_event_data_t * data = (arduino_usb_cdc_event_data_t*)event_data;
    switch (event_id){
      case ARDUINO_USB_CDC_CONNECTED_EVENT:
        HWSerial.println("CDC CONNECTED");
  
        break;
      case ARDUINO_USB_CDC_DISCONNECTED_EVENT:
        HWSerial.println("CDC DISCONNECTED");
        break;
      case ARDUINO_USB_CDC_LINE_STATE_EVENT:
        HWSerial.printf("CDC LINE STATE: dtr: %u, rts: %u, writeAvail: %u \n", data->line_state.dtr, data->line_state.rts, USBSerial.availableForWrite());       
        //if(data->line_state.rts) USBSerial.write(64);
        break;
        
      case ARDUINO_USB_CDC_LINE_CODING_EVENT:
        HWSerial.printf("CDC LINE CODING: bit_rate: %u, data_bits: %u, stop_bits: %u, parity: %u\n", data->line_coding.bit_rate, data->line_coding.data_bits, data->line_coding.stop_bits, data->line_coding.parity);
        break;
        
      case ARDUINO_USB_CDC_RX_EVENT:
        HWSerial.printf("CDC RX [%u]:", data->rx.len);
        {
            uint8_t buf[data->rx.len];
            size_t len = USBSerial.read(buf, data->rx.len);
            
            //HWSerial.write(buf, len);
            for(int i=0; i < len; ++i) HWSerial.printf("0x%X(%c),", buf[i], buf[i]);
            HWSerial.println();
            
            uint8_t transferConfirm[] = {'S', '0', '1'};            
            if(buf[0] == 0x76) {        
              //USBSerial._onLineState(true, true);
              HWSerial.printf("bytes written: %i \n", USBSerial.write('1'));
            }
           /*
            uint8_t transferHello[] = {'H', 'i'};            
            if(buf[0] == 'a') HWSerial.printf("bytes written: %i \n", USBSerial.write(transferHello, sizeof(transferHello)));
    
            uint8_t transferConfirm[] = {'S', '0', '1'};            
            if(buf[0] == 0x75) {        
              //USBSerial._onLineState(true, true);
              //HWSerial.printf("bytes written: %i \n", USBSerial.write(transferConfirm, sizeof(transferConfirm)));
            }
            
            uint8_t transferSize[] = {64};            
            if(buf[0] == 0x03) {
              USBSerial._onLineState(true, true);
              //HWSerial.printf("bytes written: %i \n", USBSerial.write(transferSize, sizeof(transferSize)));
            }
            
            //USBSerial.flush();   
*/
            break;
        }
        
       case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT:
        HWSerial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes);
        break;
     
      default:
        break;
    }
  }
}

void setup() {
  HWSerial.begin(115200);
  HWSerial.setDebugOutput(true);
  
  USB.onEvent(usbEventCallback);
  USBSerial.onEvent(usbEventCallback);
  USBSerial.setDebugOutput(true);
  USBSerial.begin();
  USB.begin();

  HWSerial.println("UsbCdc Started");
}

void loop() {
/*
  delay(1000);
  //USBSerial.println("HELLO!");
  
  while(HWSerial.available()){
    size_t l = HWSerial.available();
    uint8_t b[l];
    l = HWSerial.read(b, l);
    //USBSerial.write(b, l);
  }
*/
}
#endif /* ARDUINO_USB_MODE */