TARGET	:=	MD2Combiner.exe

CC		:=	gcc

ODIR	:=	build
SDIR	:=	source
IDIR	:=	include

CFLAGS	:=	-I$(IDIR) -Wall

CFILES	:=	$(wildcard $(SDIR)/*.c)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(wildcard $(SDIR)/*.c))

$(TARGET): $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o $(TARGET) $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	@rm -f $(TARGET) $(ODIR)/*.o
	@rmdir $(ODIR)
