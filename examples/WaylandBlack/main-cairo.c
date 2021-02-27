#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include <syscall.h>
#include <unistd.h>
#include <sys/mman.h>
#include <xkbcommon/xkbcommon.h>

#include <cairo/cairo.h>
#include <linux/input-event-codes.h>

#include <math.h>
#include <wayland-client.h>
#include "xdg-shell.h"

struct wl_compositor *compositor;
struct wl_shm *shm;
struct wl_shell *shell;
struct wl_shell_surface *shell_surface;

struct zxdg_shell_v6 *xdg_shell;
struct zxdg_surface_v6 *xdg_surface;
struct zxdg_toplevel_v6 *xdg_toplevel;

static struct xkb_keymap *keymap = NULL;
static struct xkb_context *xkb_context;
static struct xkb_state *xkb_state = NULL;

struct wl_seat *seat;
struct wl_pointer *pointer;
struct wl_keyboard *keyboard;

static int ix=100, iy=100, i=0;
int fd, size;
int width = 500;
int height = 500;
int stride; 
struct wl_shm_pool *pool;
struct wl_display *display;
struct wl_surface *surface;

unsigned char *data;
struct wl_buffer *buffer;
struct wl_buffer *bufferN;

cairo_device_t *device;
cairo_surface_t *csurface;
cairo_t *cr;
cairo_format_t cairo_format = CAIRO_FORMAT_ARGB32;
static const cairo_user_data_key_t shm_sdk;
int nredraw = 0;

void draw_text(){
  cr=cairo_create(csurface);//cairo_reference
  cairo_set_source_surface(cr, csurface, 0 , 0);
  cairo_set_source_rgba(cr,0.0,1,0.8,0.95);
  cairo_move_to(cr, ix, iy);
  cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL,
			 CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 15);
  cairo_show_text(cr, "Howdy!");
  printf ("Drawing %d %d %d %d \n", ix, iy, width, height);
  cairo_surface_flush(csurface);
  cairo_destroy(cr);
  wl_surface_attach(surface, buffer, 0, 0);
  wl_surface_commit(surface);
}

void draw_path(){
  cr=cairo_create(csurface);//cairo_reference
  cairo_set_source_rgba(cr,1.0,0.2,0.2,1.0);
  cairo_move_to(cr, ix, iy);

  for (int i=0;i<360;i++){
    int dy=(int)(iy-sin(i*M_PI/180)*50);
    cairo_line_to(cr, ix+i, dy);
  }
  cairo_stroke(cr);
  cairo_surface_flush(csurface);
  cairo_destroy(cr);  
  wl_surface_attach(surface, buffer, 0, 0);
  wl_surface_commit(surface);
}


static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
				   uint32_t format, int fd, uint32_t size){
  char *keymap_string = mmap (NULL, size, PROT_READ, MAP_SHARED, fd, 0);
  xkb_keymap_unref (keymap);
  keymap = xkb_keymap_new_from_string (xkb_context, keymap_string,
				       XKB_KEYMAP_FORMAT_TEXT_V1,
				       XKB_KEYMAP_COMPILE_NO_FLAGS);
  munmap (keymap_string, size);
  close (fd);
  xkb_state_unref (xkb_state);
  xkb_state = xkb_state_new (keymap);
}

static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
				  uint32_t serial, struct wl_surface *surface,
				  struct wl_array *keys){
  fprintf(stderr, "Keyboard gained focus\n");
}

static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
				  uint32_t serial, struct wl_surface *surface){
  fprintf(stderr, "Keyboard lost focus\n");
}

static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
				uint32_t serial, uint32_t time, uint32_t key,
				uint32_t state){
  if (state == WL_KEYBOARD_KEY_STATE_PRESSED){
    xkb_keysym_t keysym = xkb_state_key_get_one_sym (xkb_state, key+8);
    char name[64];
    xkb_keysym_get_name (keysym, name, 64);
    printf ("the key =>%s<= was pressed\n", name);
    if (strcmp(name, "q")==0) exit (0);
  }
}

static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
				      uint32_t serial, uint32_t mods_depressed,
				      uint32_t mods_latched,
				      uint32_t mods_locked,uint32_t group){
  fprintf(stderr, "Modifiers depressed %d, latched %d, locked %d, group %d\n",
	  mods_depressed, mods_latched, mods_locked, group);
}

static const struct wl_keyboard_listener keyboard_listener = {
  keyboard_handle_keymap,
  keyboard_handle_enter,
  keyboard_handle_leave,
  keyboard_handle_key,
  keyboard_handle_modifiers,
};


static void handle_ping(void *data, struct zxdg_shell_v6 *surface,
			uint32_t serial) {
  zxdg_shell_v6_pong(surface, serial);
  fprintf(stderr, "Pinged and ponged zxdg\n");
}



static void pointer_handle_leave(void *data, struct wl_pointer *pointer,
				 uint32_t serial, struct wl_surface *surface){
  //fprintf(stderr, "Pointer left surface %p\n", surface);
}

//Zapomnimo pozicijo, pri button_event ni pozicije, to je obravnavano
//v okviru zadnjega enter/motion eventa!!!
static void pointer_handle_enter(void *data, struct wl_pointer *pointer,
				 uint32_t serial, struct wl_surface *surface,
				 wl_fixed_t sx, wl_fixed_t sy){
  //fprintf(stderr, "Pointer entered surface %p at %d %d\n", surface, sx, sy);
  ix=wl_fixed_to_int(sx);
  iy=wl_fixed_to_int(sy);
}


static void pointer_handle_motion(void *data, struct wl_pointer *pointer,
				  uint32_t time, wl_fixed_t sx, wl_fixed_t sy){
  //printf("Pointer moved %d %d\n", wl_fixed_to_int(sx), wl_fixed_to_int(sy));
  ix=wl_fixed_to_int(sx);
  iy=wl_fixed_to_int(sy);
}

static void pointer_handle_button(void *data, struct wl_pointer *ptr,
				  uint32_t serial, uint32_t time,
				  uint32_t button, uint32_t state){
  printf("Pointer button: 0x%x state: %d\n",button, state);
  if (button == BTN_MIDDLE && state == 1)
    zxdg_toplevel_v6_move(xdg_toplevel, seat, serial);
  else if (button == BTN_LEFT && state == 1) draw_text();
  else if (button == BTN_RIGHT && state == 1) draw_path();
}

//dogodki z uporabo kolescka na miski - horizontalen ali vertikalen
static void pointer_handle_axis(void *data, struct wl_pointer *wl_pointer,
				uint32_t time, uint32_t axis, wl_fixed_t value){
  printf("Pointer handle axis\n");
}

const struct zxdg_shell_v6_listener xdg_shell_surface_listener =
  {
   .ping=handle_ping
  };


static void handle_ping_wl(void *data, struct wl_shell_surface *surface,
			uint32_t serial) {
  wl_shell_surface_pong(surface, serial);
  fprintf(stderr, "Pinged and ponged\n");
}
static void
handle_configure(void *data, struct wl_shell_surface *shell_surface,
		 uint32_t edges, int32_t width, int32_t height){}

static void
handle_popup_done(void *data, struct wl_shell_surface *shell_surface){}


const struct wl_shell_surface_listener shell_surface_listener =
  {
   handle_ping_wl,
   handle_configure,
   handle_popup_done
  };


static const struct wl_pointer_listener pointer_listener =
  {
   .enter=pointer_handle_enter,
   .leave=pointer_handle_leave,
   .motion=pointer_handle_motion,
   .button=pointer_handle_button,
   .axis=pointer_handle_axis,
  };

// vsi input eventi, ki lahko pridejo iz enega sedaza, za katerim
// sedi uporabnik
static void seat_handle_capabilities(void *data, struct wl_seat *seat,
				     enum wl_seat_capability caps){
  if ((caps & WL_SEAT_CAPABILITY_POINTER) && !pointer) {
    pointer = wl_seat_get_pointer(seat);
    wl_pointer_add_listener(pointer, &pointer_listener, NULL);
  } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && pointer) {
    wl_pointer_destroy(pointer);
    pointer = NULL;
  }
  if (caps & WL_SEAT_CAPABILITY_KEYBOARD) {
    keyboard = wl_seat_get_keyboard(seat);
    wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
  } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD)) {
    wl_keyboard_destroy(keyboard);
    keyboard = NULL;
  }
}

static const struct wl_seat_listener seat_listener =
  {
   seat_handle_capabilities, // lahko tudi klicanje te metode iz main 
  };


void xdg_toplevel_configure_handler(void *data,
				    struct zxdg_toplevel_v6 *xdg_toplevel,
				    int32_t width, int32_t height,
				    struct wl_array *states) {
  printf("configure: %dx%d\n", width, height);
}

void xdg_toplevel_close_handler(void *data,
				struct zxdg_toplevel_v6 *xdg_toplevel) {
  printf("close\n");
}

const struct zxdg_toplevel_v6_listener xdg_toplevel_listener =
  {
   .configure = xdg_toplevel_configure_handler,
   .close = xdg_toplevel_close_handler
  };

void xdg_surface_configure_handler(void *data,
				   struct zxdg_surface_v6 *xdg_surface,
				   uint32_t serial) {
  zxdg_surface_v6_ack_configure(xdg_surface, serial);
}

const struct zxdg_surface_v6_listener xdg_surface_listener =
  {
   .configure = xdg_surface_configure_handler
  };

void registry_global_handler(void *data, struct wl_registry *registry,
			     uint32_t name, const char *interface,
			     uint32_t version) {
  printf("Got a registry event for %s id %d\n", interface, name);
  if (strcmp(interface, "wl_compositor") == 0) {
    compositor = wl_registry_bind(registry, name,
				  &wl_compositor_interface, 3);
  } else if (strcmp(interface, "wl_shm") == 0) {
    shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
  } else if (strcmp(interface, "wl_seat") == 0) {
    seat = wl_registry_bind(registry, name, &wl_seat_interface, 1);
    wl_seat_add_listener(seat, &seat_listener, NULL);
  } else if (strcmp(interface, "zxdg_shell_v6") == 0) {
    xdg_shell = wl_registry_bind(registry, name,
				 &zxdg_shell_v6_interface, 1);
  }
}

void registry_global_remove_handler(void *data, struct wl_registry *registry,
				    uint32_t name) {}



const struct wl_registry_listener registry_listener =
  {
   .global = registry_global_handler,
   .global_remove = registry_global_remove_handler
  };



void draw_cairo(){
  cr=cairo_create(csurface);
  cairo_set_source_rgba(cr,1,1,0.8,0.4);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_fill(cr);
  cairo_move_to(cr, 0, height);
  cairo_show_text(cr,"Initializing surface");
  cairo_destroy(cr);
  cairo_surface_flush(csurface);
  wl_surface_attach(surface, buffer, 0, 0);
  wl_surface_commit(surface);
}

int main(void){
  display = wl_display_connect(NULL);
  struct wl_registry *registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, NULL);

  wl_display_roundtrip(display);

  xkb_context = xkb_context_new (XKB_CONTEXT_NO_FLAGS);
  
  surface = wl_compositor_create_surface(compositor);
  
  xdg_surface = zxdg_shell_v6_get_xdg_surface(xdg_shell, surface);
  zxdg_surface_v6_add_listener(xdg_surface, &xdg_surface_listener, NULL);
  xdg_toplevel = zxdg_surface_v6_get_toplevel(xdg_surface);
  zxdg_toplevel_v6_add_listener(xdg_toplevel, &xdg_toplevel_listener, NULL);
  zxdg_shell_v6_add_listener(xdg_shell, &xdg_shell_surface_listener, NULL);
  
  wl_surface_commit(surface);

  stride = width * 4;
  size = stride * height;  // bytes

  // odpri anonimno datoteko v RAMu in zapisi nekaj nicel 
  fd = syscall(SYS_memfd_create, "buffer", 0);
  ftruncate(fd, size * 2);

  // mapiraj v pomnilnik, data kaze na zacetek te pomniliske lokacije 
  data = mmap(NULL, size * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  // pretvori v deljeni pomnilnik 
  pool = wl_shm_create_pool(shm, fd, size * 2);

  //alociraj bufer v pomnilniku
  buffer=wl_shm_pool_create_buffer(pool, 0, width, height,
				   stride,WL_SHM_FORMAT_ARGB8888);
  bufferN=wl_shm_pool_create_buffer(pool, size, width, height,
				   stride,WL_SHM_FORMAT_ARGB8888);
  
  // kreiraj povrsino za cairo
  csurface = cairo_image_surface_create_for_data (data, cairo_format, width,
						  height, stride);
  
  // pocakaj, da se vsi zahtevki izvedejo
  wl_display_roundtrip(display);

  /* izrisemo in prikazemo*/
  draw_cairo();
  draw_text();
  
  while (1) {
    wl_display_dispatch(display);
  }
}
  
