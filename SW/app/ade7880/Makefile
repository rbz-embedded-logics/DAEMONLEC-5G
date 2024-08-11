DEPS = 
SYSROOT = ${CURDIR}/../../env/toolchain/sysroots/armv8a-poky-linux/

CFLAGS = -I$(SYSROOT)/usr/include/glib-2.0 -I$(SYSROOT)/usr/lib/glib-2.0/include -I$(SYSROOT)/usr/include/libxml2 -I$(SYSROOT)/usr/include -I. -I./include/ --sysroot=$(SYSROOT)
LFLAGS = -lgobject-2.0 -lgmodule-2.0 -lxml2 -lgthread-2.0 -lrt -lglib-2.0 -lm 

OBJ = ade7880_spi.o meter_app.o
OBJ_INIT = ade7880_spi.o meter_init.o
OBJ_READ = ade7880_spi.o meter_read.o
OBJ_CALIB = ade7880_spi_calib.o meter_calib.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

meter_app: $(OBJ)
	$(CC) -o meter_app $(CFLAGS) $(OBJ) $(LFLAGS)

meter_init: $(OBJ_INIT)
	$(CC) -o meter_init $(CFLAGS) $(OBJ_INIT) $(LFLAGS)

meter_read: $(OBJ_READ)
	$(CC) -o meter_read $(CFLAGS) $(OBJ_READ) $(LFLAGS)

meter_calib: $(OBJ_CALIB)
	$(CC) -o meter_calib $(CFLAGS) $(OBJ_CALIB) $(LFLAGS)

clean:
	rm *.o meter_app meter_init meter_read meter_calib
