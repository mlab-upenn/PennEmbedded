/*****************************************
 To understand the various parts of this code, you must understand the anatomy of a Pebble WatchApp. 
 Go through this page to undersatnd them: https://developer.getpebble.com/2/guides/anatomy-of-pebble-application.html
 *****************************************/


#include <pebble.h>
#include <stdlib.h>


#define MY_UUID {0xF0, 0x73, 0x4B, 0x0E, 0x18, 0x9E, 0x43, 0x99, 0xB2, 0xA0, 0xF1, 0x61, 0x6A, 0xDD, 0xE5, 0x54}


static Window *window;
static TextLayer *text_layer;

/*
 This function is called when an outgoing message is delivered to the App on the Paired Device
 */
static void out_sent_handler(DictionaryIterator *sent, void *context) {
    // outgoing message was delivered
    text_layer_set_text(text_layer, "Sent");//Function to display content on the WatchApp
}

/*
 This function is called when an outgoing message fails to get delivered to the App on the Paired Device
 */
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
    text_layer_set_text(text_layer, "Failed");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", reason);//This can be used to display logs on your machine while the WatchApp runs  
}

/*
 This function is called when there is an incoming message from the App on the Paired Device
 */
static void in_received_handler(DictionaryIterator *received, void *context) {
 
}

/*
 This function is called when an incoming message from the App on the Paired Device is dropped
 */
static void in_dropped_handler(AppMessageResult reason, void *context) {
    // handling incoming message drop event
}

/*
 This function receives the data to be sent from the click handlers and sends it over the Bluetooth interface.
 Follow this link for details: https://developer.getpebble.com/2/guides/app-phone-communication.html
 */
static void send_cmd(int key) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet value = TupletInteger(key,0);
    dict_write_tuplet(iter, &value);
    
    app_message_outbox_send();
    
    if (key == 0)
        text_layer_set_text(text_layer, "Go Forward");
    else if (key == 1)
        text_layer_set_text(text_layer, "Go  Left");
    else
        text_layer_set_text(text_layer, "Go Right");
}


/*
 This function is called when the select (middle) button is pressed on the Pebble.
 Send appropriate command. '0' in this case.
 */
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    send_cmd(0);
    //text_layer_set_text(text_layer, c);
}


/*
 This function is called when the up button is pressed on the Pebble.
 Press appropriate message. '1' in this case.
 */
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    send_cmd(1);
}



/*
This function is called when the down button is pressed on the Pebble.
Press appropriate message. '2' in this case.
 */
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    send_cmd(2);
}



static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}


static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  const bool animated = true;
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
    
    
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    
  app_event_loop();
  deinit();
}
