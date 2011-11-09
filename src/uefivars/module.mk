uefivars_SRCDIR     := src/uefivars

uefivars_OBJECTS    := uefivars.o
uefivars_TARGETS    := uefivars
uefivars_FULLTARGET :=  \
	$(patsubst %, $(uefivars_SRCDIR)/%, $(uefivars_TARGETS))
uefivars_FULLOBJECT :=  \
	$(patsubst %, $(uefivars_SRCDIR)/%, $(uefivars_OBJECT))

uefivars_LIBS       := efi.o efichar.o unparse_path.o efivars_procfs.o efivars_sysfs.o eficore.o disk.o scsi_ioctls.o gpt.o
uefivars_LIBDIR     := src/lib
uefivars_FULLLIB    := \
	$(patsubst %,$(uefivars_LIBDIR)/%,$(uefivars_LIBS))
LIBS = -lpci -lz

ALLDEPS += $(uefivars_FULLTARGET)
CLEANLIST += $(uefivars_FULLTARGET)
CLEANLIST += $(uefivars_FULLOBJECT)
bindir_TARGETS += $(uefivars_FULLTARGET)

$(uefivars_FULLTARGET): \
	$(uefivars_FULLOBJECT) \
	$(uefivars_FULLLIB)
	$(CC) $(CFLAGS) $(LDFLAGS) $(uefivars_SRCDIR)/uefivars.c $^ $(LIBS) -o $@
