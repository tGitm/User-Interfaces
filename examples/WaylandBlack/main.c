#include <stdio.h>
#include <string.h>

#include <syscall.h>
#include <unistd.h>
#include <sys/mman.h>



#include <wayland-client.h>
#include "xdg-shell.h"

struct wl_compositor *compositor;
struct wl_shm *shm;
struct wl_shell *shell;
struct zxdg_shell_v6 *xdg_shell;

struct wl_seat *seat;
struct wl_pointer *pointer;
static int ix=0, iy=0, i=0;
int fd, size;
int width = 256;
int height = 256;
int stride; 
struct wl_shm_pool *pool;
struct wl_display *display;
struct wl_surface *surface;

unsigned char *data;
struct wl_buffer *buffer;

void draw(){  
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      struct pixel { // kreiramo kazalec na strukturo 
	unsigned char blue, green, red, alpha;
      } *px;
      // spremenljivka kaze na del v pomnilniku
      px = (struct pixel *) (data + y * stride + x * 4);
      px->alpha = 255;
      px->red = x;
      px->green = y;
      px->blue = i;	
    }
  }
}

void redraw(){
  i=(i++)%256;
  draw(); 

  wl_surface_attach(surface, buffer, 0, 0);
  wl_surface_commit(surface);
}


static void handle_ping(void *data, struct zxdg_shell_v6 *surface,
			uint32_t serial) {
  zxdg_shell_v6_pong(surface, serial);
  fprintf(stderr, "Pinged and ponged\n");
}

static void pointer_handle_enter(void *data, struct wl_pointer *pointer,
				 uint32_t serial, struct wl_surface *surface,
				 wl_fixed_t sx, wl_fixed_t sy){
  fprintf(stderr, "Pointer entered surface %p at %d %d\n", surface, sx, sy);
}

static void pointer_handle_leave(void *data, struct wl_pointer *pointer,
				 uint32_t serial, struct wl_surface *surface){
  fprintf(stderr, "Pointer left surface %p\n", surface);
}

static void pointer_handle_motion(void *data, struct wl_pointer *pointer,
				  uint32_t time, wl_fixed_t sx, wl_fixed_t sy){
  printf("Pointer moved at %d %d\n", wl_fixed_to_int(sx), wl_fixed_to_int(sy));
  i++;
  ix=wl_fixed_to_int(sx);
  iy=wl_fixed_to_int(sy);
}

static void pointer_handle_button(void *data, struct wl_pointer *ptr,
				  uint32_t serial, uint32_t time,
				  uint32_t button, uint32_t state){
  printf("Pointer button: 0x%x state: %d\n",button, state);
  redraw();
}

static void pointer_handle_axis(void *data, struct wl_pointer *wl_pointer,
				uint32_t time, uint32_t axis, wl_fixed_t value){
  printf("Pointer handle axis\n");
}

const struct zxdg_shell_v6_listener xdg_shell_surface_listener = {
  .ping=handle_ping
};


static const struct wl_pointer_listener pointer_listener = {
  .enter=pointer_handle_enter,
  .leave=pointer_handle_leave,
  .motion=pointer_handle_motion,
  .button=pointer_handle_button,
  .axis=pointer_handle_axis,
};

static void seat_handle_capabilities(void *data, struct wl_seat *seat,
				     enum wl_seat_capability caps){
  if ((caps & WL_SEAT_CAPABILITY_POINTER) && !pointer) {
    pointer = wl_seat_get_pointer(seat);
    wl_pointer_add_listener(pointer, &pointer_listener, NULL);
  } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && pointer) {
    wl_pointer_destroy(pointer);
    pointer = NULL;
  }
}

static const struct wl_seat_listener seat_listener = {
  seat_handle_capabilities, // lahko tudi klicanje te metode iz main 
};


void xdg_toplevel_configure_handler(void *data, struct zxdg_toplevel_v6 *xdg_toplevel, int32_t width,
				    int32_t height, struct wl_array *states) {
  printf("configure: %dx%d\n", width, height);
}

void xdg_toplevel_close_handler(void *data, struct zxdg_toplevel_v6 *xdg_toplevel) {
  printf("close\n");
}

const struct zxdg_toplevel_v6_listener xdg_toplevel_listener = {
  .configure = xdg_toplevel_configure_handler,
  .close = xdg_toplevel_close_handler
};

void xdg_surface_configure_handler(void *data, struct zxdg_surface_v6 *xdg_surface, uint32_t serial) {
  zxdg_surface_v6_ack_configure(xdg_surface, serial);
}

const struct zxdg_surface_v6_listener xdg_surface_listener = {
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
  } else if (strcmp(interface, "wl_shell") == 0) {
    shell = wl_registry_bind(registry, name,
			     &wl_shell_interface, 1);
  } else if (strcmp(interface, "zxdg_shell_v6") == 0) {
    xdg_shell = wl_registry_bind(registry, name,
				 &zxdg_shell_v6_interface, 1);
  }
}

void registry_global_remove_handler(void *data, struct wl_registry *registry,
				    uint32_t name) {}



const struct wl_registry_listener registry_listener = {
  .global = registry_global_handler,
  .global_remove = registry_global_remove_handler
};


int main(void){
  display = wl_display_connect(NULL);
  struct wl_registry *registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, NULL);

  wl_display_roundtrip(display);

  surface = wl_compositor_create_surface(compositor);
  struct zxdg_surface_v6 *xdg_surface = zxdg_shell_v6_get_xdg_surface(xdg_shell, surface);
  zxdg_surface_v6_add_listener(xdg_surface, &xdg_surface_listener, NULL);
  struct zxdg_toplevel_v6 *xdg_toplevel = zxdg_surface_v6_get_toplevel(xdg_surface);
  zxdg_toplevel_v6_add_listener(xdg_toplevel, &xdg_toplevel_listener, NULL);
  //struct wl_shell_surface *shell_surface = wl_shell_get_shell_surface(shell, surface);
  //wl_shell_surface_set_toplevel(shell_surface);
  zxdg_shell_v6_add_listener(xdg_shell, &xdg_shell_surface_listener, NULL);
  
  wl_surface_commit(surface);

  stride = width * 4;
  size = stride * height;  // bytes

  // odpri anonimno datoteko v RAMu in zapisi nekaj nicel 
  fd = syscall(SYS_memfd_create, "buffer", 0);
  ftruncate(fd, size);

  // mapiraj v pomnilnik, data kaze na zacetek te pomniliske lokacije 
  data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  // pretvori v deljeni pomnilnik 
  pool = wl_shm_create_pool(shm, fd, size);

  //alociraj bufer v pomnilniku
  buffer=wl_shm_pool_create_buffer(pool,0, width, height,
				   stride,
				   WL_SHM_FORMAT_ARGB8888);
  
  /* izrisemo*/
  draw(); 


  wl_display_roundtrip(display);

  /*prikazemo izrisano*/
  wl_surface_attach(surface, buffer, 0, 0);
  wl_surface_commit(surface);

  while (1) {
    wl_display_dispatch(display);
  }
}
