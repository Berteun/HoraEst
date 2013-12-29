#include <pebble.h>
#include "roman_date.h"
#include "roman_numerals.h"

static struct StaticData {
  TextLayer *date_label;
  TextLayer *time_label;
  Layer  *line_layer;
  Window *window;
  /* Longest is:  XVIII:XXXVIII */
  char time_buffer[14];
  /* Longest is (e.g.) 'post Kal Apr' */
  char date_buffer[20];
} s_data;

static void line_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void update_time(struct tm* t) {
	s_data.time_buffer[0] = '\0';
	to_roman_numerals(t->tm_hour, s_data.time_buffer);
	strcat(s_data.time_buffer, ":");
	to_roman_numerals(t->tm_min, s_data.time_buffer);

	text_layer_set_text(s_data.time_label, s_data.time_buffer);

	s_data.date_buffer[0] = '\0';
	date_to_roman(t, s_data.date_buffer);
	text_layer_set_text(s_data.date_label, s_data.date_buffer);
}

static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void do_init(void) {
  s_data.window = window_create();
  window_stack_push(s_data.window, true /* animated */);
  window_set_background_color(s_data.window, GColorBlack);

  Layer *root_layer = window_get_root_layer(s_data.window);

  /* Same layout as Simplicity example. */
  s_data.date_label = text_layer_create(GRect(8, 68, 144-8, 168-68));
  text_layer_set_text_color(s_data.date_label, GColorWhite);
  text_layer_set_background_color(s_data.date_label, GColorClear);
  text_layer_set_font(s_data.date_label, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_20)));
  layer_add_child(root_layer, text_layer_get_layer(s_data.date_label));

  s_data.time_label = text_layer_create(GRect(7, 92, 144 - 7, 168 - 92));
  text_layer_set_text_color(s_data.time_label, GColorWhite);
  text_layer_set_background_color(s_data.time_label, GColorClear);
  text_layer_set_font(s_data.time_label, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_25)));
  layer_add_child(root_layer, text_layer_get_layer(s_data.time_label));

  GRect line_frame = GRect(8, 97, 139, 2);
  s_data.line_layer = layer_create(line_frame);
  layer_set_update_proc(s_data.line_layer, &line_layer_update_callback);
  layer_add_child(root_layer, s_data.line_layer);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  update_time(t);
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
}

static void do_deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_data.window);
  text_layer_destroy(s_data.time_label);
  text_layer_destroy(s_data.date_label);
  layer_destroy(s_data.line_layer);
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}

/* vim: set ts=8 noet sw=8 cc=80 tw=79 number: */
