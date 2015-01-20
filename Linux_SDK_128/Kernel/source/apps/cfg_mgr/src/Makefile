CFG_MGR = cfg_mgr		# Application Name

BASE_DIR = $(PWD)
SRC_DIRS = .
OBJ_DIR  = ../obj

#CFLAGS  += -I ../../../common
CFLAGS  += -I../inc
LDFLAGS += #-L../ fix me

C_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJFILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(C_FILES))

all: $(CFG_MGR)

$(CFG_MGR) : $(OBJFILES)
	$(CC) $(OBJFILES) $(CFLAGS) $(LDFLAGS) -s -o $@ -lconn -lpthread 

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(OBJFILES): | OBJ

OBJ:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/
	rm -f $(CFG_MGR)
