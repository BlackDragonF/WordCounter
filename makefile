SRC_DIR = src
INC_DIR = include
OBJ_DIR = objs
BIN_DIR = bin
DIRS := $(OBJ_DIR) $(BIN_DIR)

CC = gcc
CFLAGS = -I $(INC_DIR)
RM = rm -f
MKDIR = mkdir

SRCS = $(wildcard ${SRC_DIR}/*.c)
OBJS = $(patsubst %.c, ${OBJ_DIR}/%.o, $(notdir ${SRCS}))
HEADERS = $(wildcard ${INC_DIR}/*.h)
TARGET = $(BIN_DIR)/wordcounter

all : $(DIRS) $(TARGET)

$(DIRS) :
	$(MKDIR) $@

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJS) : $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean :
	$(RM) $(OBJS) $(TARGET)
