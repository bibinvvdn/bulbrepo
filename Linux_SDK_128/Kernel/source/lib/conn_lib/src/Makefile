CONN_LIB = libconn.a		# Conn Lib

SRC_DIRS = .
OBJ_DIR  = ../obj
INC_DIR = ../inc
CFLAGS += -I$(INC_DIR)

C_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJFILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(C_FILES))

AR_FLAGS += rs

$(CONN_LIB) : $(OBJFILES)
	$(AR) $(AR_FLAGS) $(CONN_LIB) $(OBJFILES)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJFILES): | OBJ

OBJ:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(CONN_LIB)
