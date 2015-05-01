

CC     = gcc
AT     = @
MKDIR  = mkdir -p
RM     = rm -rf

#------------------
# User setting
#------------------

USE_DEBUG=yes

TARGET=expat_sample

INCLUDES  = -I./include
CFLAGS    = -Wall
LDFLAGS   = -lpthread -lexpat -ldl -Xlinker --export-dynamic

ifeq ($(USE_DEBUG), yes)
CFLAGS += -g
endif


OUTPUT_DIR = work
SRCS       = $(shell find * -name "*.c")

source-dir-to-binary-dir = $(addprefix $(OUTPUT_DIR)/, $1)
source-to-object = $(call source-dir-to-binary-dir, \
	$(patsubst %.c, %.o, $(SRCS)))                  \

objects     := $(call source-to-object)
source-dirs := $(sort $(dir $(call source-to-object)))
create-output-directory :=                  \
	$(shell for f in $(call source-dirs);   \
	do                                      \
		test -d $$f || $(MKDIR) $$f;        \
	done)


.PHONY: all clean

all:$(TARGET)

$(TARGET):$(objects)
	$(CC) -o $@ $^ $(LDFLAGS)


$(OUTPUT_DIR)/%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<


clean:
	$(AT)$(RM) $(OUTPUT_DIR) $(TARGET)
