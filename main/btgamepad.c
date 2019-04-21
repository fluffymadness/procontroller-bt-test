#define __BTSTACK_FILE__ "btgamepad.c"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "esp_system.h"
#include "btstack.h"

static const char hid_device_name[] = "Pro Controller";
static const char service_name[] = "Wireless Gamepad";

static btstack_packet_callback_registration_t hci_event_callback_registration;
static uint16_t hid_cid;
static uint16_t pairing_state;

static uint8_t hid_service_buffer[450];
static uint8_t device_id_sdp_service_buffer[200];
static uint8_t l2cap_sdp_service_buffer[600];

/* USB Pro Controller Report Map
const uint8_t reportMap[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x15, 0x00,        // Logical Minimum (0)
0x09, 0x04,        // Usage (Joystick)
0xA1, 0x01,        // Collection (Application)
0x85, 0x30,        //   Report ID (48)
0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
0x05, 0x09,        //   Usage Page (Button)
0x19, 0x01,        //   Usage Minimum (0x01)
0x29, 0x0A,        //   Usage Maximum (0x0A)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x0A,        //   Report Count (10)
0x55, 0x00,        //   Unit Exponent (0)
0x65, 0x00,        //   Unit (None)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x09,        //   Usage Page (Button)
0x19, 0x0B,        //   Usage Minimum (0x0B)
0x29, 0x0E,        //   Usage Maximum (0x0E)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x04,        //   Report Count (4)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x01,        //   Report Size (1)
0x95, 0x02,        //   Report Count (2)
0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x0B, 0x01, 0x00, 0x01, 0x00,  //   Usage (0x010001)
0xA1, 0x00,        //   Collection (Physical)
0x0B, 0x30, 0x00, 0x01, 0x00,  //     Usage (0x010030)
0x0B, 0x31, 0x00, 0x01, 0x00,  //     Usage (0x010031)
0x0B, 0x32, 0x00, 0x01, 0x00,  //     Usage (0x010032)
0x0B, 0x35, 0x00, 0x01, 0x00,  //     Usage (0x010035)
0x15, 0x00,        //     Logical Minimum (0)
0x27, 0xFF, 0xFF, 0x00, 0x00,  //     Logical Maximum (65534)
0x75, 0x10,        //     Report Size (16)
0x95, 0x04,        //     Report Count (4)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0x0B, 0x39, 0x00, 0x01, 0x00,  //   Usage (0x010039)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x07,        //   Logical Maximum (7)
0x35, 0x00,        //   Physical Minimum (0)
0x46, 0x3B, 0x01,  //   Physical Maximum (315)
0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
0x75, 0x04,        //   Report Size (4)
0x95, 0x01,        //   Report Count (1)                               Data,Var,Abs,No Wrap,Linear,Preferred State,Null State
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x09,        //   Usage Page (Button)
0x19, 0x0F,        //   Usage Minimum (0x0F)
0x29, 0x12,        //   Usage Maximum (0x12)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x04,        //   Report Count (4)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x08,        //   Report Size (8)
0x95, 0x34,        //   Report Count (52)
0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
0x85, 0x21,        //   Report ID (33)
0x09, 0x01,        //   Usage (0x01)
0x75, 0x08,        //   Report Size (8)
0x95, 0x3F,        //   Report Count (63)
0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x85, 0x81,        //   Report ID (-127)
0x09, 0x02,        //   Usage (0x02)
0x75, 0x08,        //   Report Size (8)
0x95, 0x3F,        //   Report Count (63)
0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x85, 0x01,        //   Report ID (1)
0x09, 0x03,        //   Usage (0x03)
0x75, 0x08,        //   Report Size (8)
0x95, 0x3F,        //   Report Count (63)
0x91, 0x83,        //   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x85, 0x10,        //   Report ID (16)
0x09, 0x04,        //   Usage (0x04)
0x75, 0x08,        //   Report Size (8)
0x95, 0x3F,        //   Report Count (63)
0x91, 0x83,        //   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x85, 0x80,        //   Report ID (-128)
0x09, 0x05,        //   Usage (0x05)
0x75, 0x08,        //   Report Size (8)
0x95, 0x3F,        //   Report Count (63)
0x91, 0x83,        //   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x85, 0x82,        //   Report ID (-126)
0x09, 0x06,        //   Usage (0x06)
0x75, 0x08,        //   Report Size (8)
0x95, 0x3F,        //   Report Count (63)
0x91, 0x83,        //   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0xC0, // End Collection
  };
*/
//Bluetooth Pro Controller report map
const uint8_t reportMap[] = {
    0x05, 0x01, 
0x09, 0x05, 
0xa1, 0x01, // Collection (Application)
0x06, 0x01, 
0xff, 0x85, 
0x21, 0x09, 
0x21, 0x75, 
0x08, 0x95, 
0x30, 0x81, 
0x02, 0x85, 
0x30, 0x09, 
0x30, 0x75, 
0x08, 0x95, 
0x30, 0x81, 
0x02, 0x85, 
0x31, 0x09, 
0x31, 0x75, 
0x08, 0x96, 
0x69, 0x01, 
0x81, 0x02, 
0x85, 0x32, 
0x09, 0x32, 
0x75, 0x08, 
0x96, 0x69, 
0x01, 0x81, 
0x02, 0x85, 
0x33, 0x09, 
0x33, 0x75, 
0x08, 0x96, 
0x69, 0x01, 
0x81, 0x02, 
0x85, 0x3f, 
0x05, 0x09, 
0x19, 0x01, 
0x29, 0x10, 
0x15, 0x00, 
0x25, 0x01, 
0x75, 0x01, 
0x95, 0x10, 
0x81, 0x02, 
0x05, 0x01, 
0x09, 0x39, 
0x15, 0x00, 
0x25, 0x07, 
0x75, 0x04, 
0x95, 0x01, 
0x81, 0x42, 
0x05, 0x09, 
0x75, 0x04, 
0x95, 0x01, 
0x81, 0x01, 
0x05, 0x01, 
0x09, 0x30, 
0x09, 0x31, 
0x09, 0x33, 
0x09, 0x34, 
0x16, 0x00, 
0x00, 0x27, 
0xff, 0xff, 
0x00, 0x00, 
0x75, 0x10, 
0x95, 0x04, 
0x81, 0x02, 
0x06, 0x01, 
0xff, 0x85, 
0x01, 0x09, 
0x01, 0x75, 
0x08, 0x95, 
0x30, 0x91, 
0x02, 0x85, 
0x10, 0x09, 
0x10, 0x75, 
0x08, 0x95, 
0x30, 0x91, 
0x02, 0x85, 
0x11, 0x09, 
0x11, 0x75, 
0x08, 0x95, 
0x30, 0x91, 
0x02, 0x85, 
0x12, 0x09,
0x12, 0x75, 
0x08, 0x95, 
0x30, 0x91, 
0x02, 0xc0
};

/*

*/

typedef struct {
    uint8_t timer;
    uint8_t battery_connection_info;
    uint8_t buttons1;
    uint8_t buttons2;
    uint8_t buttons3;
    uint8_t  ldata1;     // Left  Stick
    uint8_t  ldata2;     // Left  Stick
    uint8_t  ldata3;     // Left  Stick
    uint8_t  rdata1;     // Right Stick
    uint8_t  rdata2;     // Right Stick
    uint8_t  rdata3;     // Right Stick
}
gamepad_report_t;

static gamepad_report_t joy;
uint8_t command1[] = {0xA1, 0x3F, 0x00, 0x00, 0x05, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};
uint8_t command2[] = {0xA1, 0x3F, 0x00, 0x00, 0x05, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};
uint8_t command3[] = {0xA1, 0x3F, 0x00, 0x00, 0x08, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};
uint8_t command4[] = {0xA1, 0x3F, 0x00, 0x00, 0x08, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};
uint8_t command5[] = {0xA1, 0x3F, 0x04, 0x00, 0x08, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};
uint8_t command6[] = {0xA1, 0x3F, 0x00, 0x00, 0x08, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};
static uint8_t reply02[] = {0xa1, 0x21, 0x00, 0x40, 0x00, 0x00, 0x00, 0xe6, 0x27, 0x78, 0xab, 0xd7, 0x76, 0x00, 0x82, 0x02, 0x03, 0x48, 0x03, 0x02, 0xD8, 0xA0, 0x1D, 0x40, 0x6B, 0x22, 0x03, 0x01, 0x00 , 0x00 , 0x00 , 0x00  , 0x00 , 0x00 , 0x00 , 0x00  , 0x00 , 0x00 , 0x00 , 0x00  , 0x00 , 0x00 , 0x00 , 0x00
, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };

static uint8_t reply01[] = {0xA1, 0x21, 0x1f, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x01, 0x82, 0x02, 0x03, 0x91, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


static void send_report_joystick2(){
    //0x30 is the report id , 0xa1 indicates hid data
   // joy.HAT = 0x02;
    joy.timer++;
    if(joy.timer == 255)
        joy.timer = 0;
    joy.battery_connection_info = 0x80;
    //joy.LX = 12000;
    uint8_t report[] = {
        0xa1,
        0x30,
        joy.timer,
        joy.battery_connection_info,
        joy.buttons1,
        joy.buttons2,
        joy.buttons3,
        joy.ldata1,
        joy.ldata2,
        joy.ldata3,
        joy.rdata1,
        joy.rdata2,
        joy.rdata3,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    //send report
    hid_device_send_interrupt_message(hid_cid, &report[0], sizeof(report));
}
static void send_report_joystick(){
    //0x30 is the report id , 0xa1 indicates hid data
   // joy.HAT = 0x02;
    //joy.LX = 12000;
    joy.buttons3=0x08;
    uint8_t report[] = {
        0xa1,
        0x3F,
        joy.buttons1,
        joy.buttons2,
        joy.buttons3,
        0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80
    };
    //send report
    hid_device_send_interrupt_message(hid_cid, &report[0], sizeof(report));
}
static void send_data(){
   
        if(pairing_state == 0)
            hid_device_send_interrupt_message(hid_cid, &command1[0], sizeof(command1));
            //hid_device_send_interrupt_message(hid_cid, &command1[0], sizeof(command1));
        if(pairing_state == 1)
            hid_device_send_interrupt_message(hid_cid, &command2[0], sizeof(command2));
        if(pairing_state == 3)
            hid_device_send_interrupt_message(hid_cid, &command3[0], sizeof(command3));
        if(pairing_state == 4)
            hid_device_send_interrupt_message(hid_cid, &command4[0], sizeof(command4));
        if(pairing_state == 5)
            hid_device_send_interrupt_message(hid_cid, &command5[0], sizeof(command5));
        if(pairing_state == 6)
            hid_device_send_interrupt_message(hid_cid, &command6[0], sizeof(command6));
        if(pairing_state >= 7)
            //send_report_joystick2();
            hid_device_send_interrupt_message(hid_cid, &command6[0], sizeof(command6));
        pairing_state++;
}

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t * packet, uint16_t packet_size){
    UNUSED(channel);
    UNUSED(packet_size);
    uint8_t status;
     if(packet_type == HCI_EVENT_PACKET)
    {
        switch (packet[0]){
            case BTSTACK_EVENT_STATE:
                    if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
                    break;

            case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                    // ssp: inform about user confirmation request
                    log_info("SSP User Confirmation Request with numeric value '%06"PRIu32"'\n", hci_event_user_confirmation_request_get_numeric_value(packet));
                    log_info("SSP User Confirmation Auto accept\n");
                     pairing_state = 0;
                    break;
            case HCI_EVENT_HID_META:
                switch (hci_event_hid_meta_get_subevent_code(packet)){
                    case HID_SUBEVENT_CONNECTION_OPENED:
                        status = hid_subevent_connection_opened_get_status(packet);
                        if (status) {
                            // outgoing connection failed
                            printf("Connection failed, status 0x%x\n", status);
                            hid_cid = 0;
                            return;
                        }
                        hid_cid = hid_subevent_connection_opened_get_hid_cid(packet);
                        log_info("HID Connected\n");
                        hid_device_request_can_send_now_event(hid_cid);
                        break;
                    case HID_SUBEVENT_CONNECTION_CLOSED:
                        log_info("HID Disconnected\n");
                        hid_cid = 0;
                        pairing_state = 0;
                        break;
                    case HID_SUBEVENT_CAN_SEND_NOW:  
                        if(hid_cid!=0){

                          send_report_joystick();

                          hid_device_request_can_send_now_event(hid_cid);
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                //log_info("not hid");
                break;
        }
    }
}
void device_id_sdp_record(uint8_t *service, uint32_t service_record_handle, uint16_t vendor_id_source, uint16_t vendor_id, uint16_t product_id, uint16_t version){
    //1200 Service Request: PNP info
    
    uint8_t* attribute;
    de_create_sequence(service);
    
    // 0x0000 "Service Record Handle"
    de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
    de_add_number(service, DE_UINT, DE_SIZE_32, service_record_handle);
    
    // 0x0001 "Service Class ID List"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_PNP_INFORMATION );
    }
    de_pop_sequence(service, attribute);
    //0x0004 "Protocol Descriptor List"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t * l2cpProtocol = de_push_sequence(attribute);
        {
            de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, 0x01);
        }
        de_pop_sequence(attribute, l2cpProtocol);
        
        uint8_t * hidProtocol = de_push_sequence(attribute);
        {
            de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, 0x01);
        }
        de_pop_sequence(attribute, hidProtocol);
    }
    de_pop_sequence(service, attribute);
    //0x0006 Lang Base Attribute ID
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100);
    }
    de_pop_sequence(service, attribute);
    //0x0009 Profile Descriptor List
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t * hidProfile = de_push_sequence(attribute);
        {
            de_add_number(hidProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_PNP_INFORMATION);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0100);    // Version 1.1
        }
        de_pop_sequence(attribute, hidProfile);
    }
    de_pop_sequence(service, attribute);
    //0x0100
    const char wgp2[] = "Wireless Gamepad PnP Server";
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0100);
    de_add_data(service, DE_STRING, strlen(wgp2), (uint8_t *)wgp2);
    
    //Gamepad
    const char gp2[] = "Gamepad";
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0101);
    de_add_data(service, DE_STRING, strlen(gp2), (uint8_t *)gp2);

    
    // 0x0200 "SpecificationID"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SPECIFICATION_ID);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0103);    // v1.3
    
    // 0x0201 "VendorID"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x057E);
    
    // 0x0202 "ProductID"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRODUCT_ID);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x2007);
    
    // 0x0203 "Version"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VERSION);
    de_add_number(service,  DE_UINT, DE_SIZE_16, version);
    
    // 0x0204 "PrimaryRecord"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PRIMARY_RECORD);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  1);    // yes, this is the primary record - there are no others
    
    // 0x0205 "VendorIDSource"
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_VENDOR_ID_SOURCE);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0002);
}

//Custom HID SDP record - taken from hid_device.c
void hid_sdp_record(
                           uint8_t *service,
                           uint32_t service_record_handle,
                           uint16_t hid_device_subclass,
                           uint8_t  hid_country_code,
                           uint8_t  hid_virtual_cable,
                           uint8_t  hid_reconnect_initiate,
                           uint8_t  hid_boot_device,
                           const uint8_t * descriptor, uint16_t descriptor_size,
                           const char *device_name){
    //1124 Service Request: Hunam Interface Device
    uint8_t * attribute;
    de_create_sequence(service);
    
    de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
    de_add_number(service, DE_UINT, DE_SIZE_32, service_record_handle);
    
    de_add_number(service, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE);
    }
    de_pop_sequence(service, attribute);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t * l2cpProtocol = de_push_sequence(attribute);
        {
            de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, PSM_HID_CONTROL);
        }
        de_pop_sequence(attribute, l2cpProtocol);
        
        uint8_t * hidProtocol = de_push_sequence(attribute);
        {
            de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_HIDP);
        }
        de_pop_sequence(attribute, hidProtocol);
    }
    de_pop_sequence(service, attribute);
    
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BROWSE_GROUP_LIST); // public browse group
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PUBLIC_BROWSE_ROOT );
    }
    de_pop_sequence(service, attribute);
    

    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
    attribute = de_push_sequence(service);
    {
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100);
    }
    de_pop_sequence(service, attribute);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t * hidProfile = de_push_sequence(attribute);
        {
            de_add_number(hidProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0101);    // Version 1.1
        }
        de_pop_sequence(attribute, hidProfile);
    }
    de_pop_sequence(service, attribute);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_ADDITIONAL_PROTOCOL_DESCRIPTOR_LISTS);
    attribute = de_push_sequence(service);
    {
        uint8_t * additionalDescriptorAttribute = de_push_sequence(attribute);
        {
            uint8_t * l2cpProtocol = de_push_sequence(additionalDescriptorAttribute);
            {
                de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
                de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, PSM_HID_INTERRUPT);
            }
            de_pop_sequence(additionalDescriptorAttribute, l2cpProtocol);
            
            uint8_t * hidProtocol = de_push_sequence(additionalDescriptorAttribute);
            {
                de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_HIDP);
            }
            de_pop_sequence(additionalDescriptorAttribute, hidProtocol);
        }
        de_pop_sequence(attribute, additionalDescriptorAttribute);
    }
    de_pop_sequence(service, attribute);
    
    // 0x0100 "ServiceName"
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0100);
    de_add_data(service,  DE_STRING, strlen(device_name), (uint8_t *) device_name);
    
    const char gp[] = "Gamepad";
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0101);
    de_add_data(service, DE_STRING, strlen(gp), (uint8_t *)gp);
    
    const char nin[] = "Nintendo";
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0102);
    de_add_data(service, DE_STRING, strlen(nin), (uint8_t *)nin);
    
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_PARSER_VERSION);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0111);  // v1.1.1
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DEVICE_SUBCLASS);
    de_add_number(service,  DE_UINT, DE_SIZE_8,  hid_device_subclass);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_COUNTRY_CODE);
    de_add_number(service,  DE_UINT, DE_SIZE_8,  hid_country_code);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_VIRTUAL_CABLE);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  1);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_RECONNECT_INITIATE);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  1);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_DESCRIPTOR_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t* hidDescriptor = de_push_sequence(attribute);
        {
            de_add_number(hidDescriptor,  DE_UINT, DE_SIZE_8, 0x22);    // Report Descriptor
            de_add_data(hidDescriptor,  DE_STRING, descriptor_size, (uint8_t *) descriptor);
        }
        de_pop_sequence(attribute, hidDescriptor);
    }
    de_pop_sequence(service, attribute);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HIDLANGID_BASE_LIST);
    attribute = de_push_sequence(service);
    {
        uint8_t* hig_lang_base = de_push_sequence(attribute);
        {
            // see: http://www.usb.org/developers/docs/USB_LANGIDs.pdf
            de_add_number(hig_lang_base,  DE_UINT, DE_SIZE_16, 0x0409);    // HIDLANGID = English (US)
            de_add_number(hig_lang_base,  DE_UINT, DE_SIZE_16, 0x0100);    // HIDLanguageBase = 0x0100 default
        }
        de_pop_sequence(attribute, hig_lang_base);
    }
    de_pop_sequence(service, attribute);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0209);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  1);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_REMOTE_WAKE);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  1);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x020C);
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x0C80);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, 0x20D);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  0);
    
    de_add_number(service,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_HID_BOOT_DEVICE);
    de_add_number(service,  DE_BOOL, DE_SIZE_8,  0);
}



void l2cap_create_sdp_record(void)
{
    //0100 Service Request: L2CAP
    uint8_t * attribute;
    de_create_sequence(l2cap_sdp_service_buffer);
    
    de_add_number(l2cap_sdp_service_buffer, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_RECORD_HANDLE);
    de_add_number(l2cap_sdp_service_buffer, DE_UINT, DE_SIZE_32, 0x0000);
    
    de_add_number(l2cap_sdp_service_buffer, DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SERVICE_CLASS_ID_LIST);
    attribute = de_push_sequence(l2cap_sdp_service_buffer);
    {
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, 0x1000);
    }
    de_pop_sequence(l2cap_sdp_service_buffer, attribute);
    //0x0004 "Protocol Descriptor List"
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST);
    attribute = de_push_sequence(l2cap_sdp_service_buffer);
    {
        uint8_t * l2cpProtocol = de_push_sequence(attribute);
        {
            de_add_number(l2cpProtocol,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(l2cpProtocol,  DE_UINT, DE_SIZE_16, 0x01);
        }
        de_pop_sequence(attribute, l2cpProtocol);
        
        uint8_t * hidProtocol = de_push_sequence(attribute);
        {
            de_add_number(hidProtocol,  DE_UUID, DE_SIZE_16, 0x01);
        }
        de_pop_sequence(attribute, hidProtocol);
    }
    de_pop_sequence(l2cap_sdp_service_buffer, attribute);
    // Public browse group
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BROWSE_GROUP_LIST);
    attribute = de_push_sequence(l2cap_sdp_service_buffer);
    {
        de_add_number(attribute,  DE_UUID, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_PUBLIC_BROWSE_ROOT );
    }
    de_pop_sequence(l2cap_sdp_service_buffer, attribute);
    //0x0006 Lang Base Attribute ID
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
    attribute = de_push_sequence(l2cap_sdp_service_buffer);
    {
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x656e);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x006a);
        de_add_number(attribute, DE_UINT, DE_SIZE_16, 0x0100);
    }
    de_pop_sequence(l2cap_sdp_service_buffer, attribute);
    //0x0009 Profile Descriptor List
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST);
    attribute = de_push_sequence(l2cap_sdp_service_buffer);
    {
        uint8_t * hidProfile = de_push_sequence(attribute);
        {
            de_add_number(hidProfile,  DE_UUID, DE_SIZE_16, BLUETOOTH_PROTOCOL_L2CAP);
            de_add_number(hidProfile,  DE_UINT, DE_SIZE_16, 0x0100);
        }
        de_pop_sequence(attribute, hidProfile);
    }
    de_pop_sequence(l2cap_sdp_service_buffer, attribute);
    //0x0100
    const char wgp2[] = "Wireless Gamepad";
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, 0x0100);
    de_add_data(l2cap_sdp_service_buffer, DE_STRING, strlen(wgp2), (uint8_t *)wgp2);
    
    //Gamepad
    const char gp2[] = "Gamepad";
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, 0x0101);
    de_add_data(l2cap_sdp_service_buffer, DE_STRING, strlen(gp2), (uint8_t *)gp2);
    
    // 0x0200 "SpecificationID"
    de_add_number(l2cap_sdp_service_buffer,  DE_UINT, DE_SIZE_16, BLUETOOTH_ATTRIBUTE_SPECIFICATION_ID);
    attribute = de_push_sequence(l2cap_sdp_service_buffer);
    {
        de_add_number(attribute,  DE_UINT, DE_SIZE_16, 0x0100);
    }
    de_pop_sequence(l2cap_sdp_service_buffer, attribute);
    
    //Register service
    sdp_register_service(l2cap_sdp_service_buffer);
}

int btstack_main(int argc, const char * argv[]);
int btstack_main(int argc, const char * argv[]){
    (void)argc;
    (void)argv;

    //Pro Controller MAC
    //98:B6:E9:43:26:XX
    uint8_t mac[] = {0x98, 0xB6, 0xE9, 0x43, 0x26, 0xCC};
    esp_base_mac_addr_set(mac);
    gap_discoverable_control(1);
    gap_set_class_of_device(0x2508);
    gap_set_local_name(hid_device_name);

     hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    hci_register_sco_packet_handler(&packet_handler);
    hci_dump_open(NULL, HCI_DUMP_STDOUT); 
    // L2CAP
    l2cap_init();
 //   l2cap_register_service(NULL, packet_handler, 0x11,100);


    // SDP Server
    sdp_init();
    memset(hid_service_buffer, 0, sizeof(hid_service_buffer));

    
    //1124 Service Request: Hunam Interface Device - NOTE - comment out lines in sdp_server.c sdp_register_service() to allow for 0x10000 service handles
    hid_sdp_record(hid_service_buffer, 0x10000, 0x2508, 33, 0, 0, 0, reportMap, sizeof(reportMap), "Wireless Gamepad");
    printf("HID service record size: %u\n", de_get_len( hid_service_buffer));
    

    device_id_sdp_record(device_id_sdp_service_buffer, 0x10001, DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, BLUETOOTH_COMPANY_ID_BLUEKITCHEN_GMBH, 1, 1);
    printf("Device ID SDP service record size: %u\n", de_get_len((uint8_t*)device_id_sdp_service_buffer));
    
    sdp_register_service(device_id_sdp_service_buffer);
    sdp_register_service(hid_service_buffer);
    
    //0100 Service Request: L2CAP
    l2cap_create_sdp_record();
    
    //init HID Device - hid_device.c
    hid_device_init(0, sizeof(reportMap), reportMap);
    hid_device_register_packet_handler(&packet_handler);
    
    // turn on
hci_power_control(HCI_POWER_ON);

    return 0;
}
/* LISTING_END */
/* EXAMPLE_END */
