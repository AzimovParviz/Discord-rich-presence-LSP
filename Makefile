# Compiler and linker
CC = clang
GO = go

# Directories
INCLUDE_DIR = ./discord
LIB_DIR = ./lib

# Output files
LIB_OUTPUT = libsetActivity.dylib
GO_OUTPUT = discord-lsp

# Source files
C_SRC = discord/setActivity.c
GO_SRC = main.go

# Compiler and linker flags
CFLAGS = -shared -I$(INCLUDE_DIR) -L$(LIB_DIR) -ldiscord_game_sdk
GO_LDFLAGS = -ldflags "-extldflags '-L$(LIB_DIR) -ldiscord_game_sdk -lsetActivity -rpath lib'"

# Discord Game SDK
DISCORD_SDK_URL = https://dl-game-sdk.discordapp.net/3.2.1/discord_game_sdk.zip
DISCORD_SDK_ZIP = discord_game_sdk.zip
DISCORD_SDK_DIR = discord

MACOSX_DEPLOYMENT_TARGET = 

ARCH = $(shell uname -m)
ifeq ($(ARCH),arm64)
    ARCH = aarch64
endif

UNAME = $(shell uname)

download:
	@echo "Downloading discord_game_sdk"
	curl -o $(DISCORD_SDK_ZIP) $(DISCORD_SDK_URL)
	@echo "Unzipping discord_game_sdk"
	unzip $(DISCORD_SDK_ZIP) "c/discord_game_sdk.h" -d $(DISCORD_SDK_DIR)
	mv $(DISCORD_SDK_DIR)/c/discord_game_sdk.h $(DISCORD_SDK_DIR)/discord_game_sdk.h
ifeq ($(UNAME),Darwin)
	@echo "Unzipping discord_game_sdk.dylib"
	unzip $(DISCORD_SDK_ZIP) "lib/$(ARCH)/discord_game_sdk.dylib" -d $(LIB_DIR)
	mv $(LIB_DIR)/lib/$(ARCH)/discord_game_sdk.dylib $(LIB_DIR)/libdiscord_game_sdk.dylib
	rm -r $(LIB_DIR)/lib
	-ln -s $(LIB_DIR)/libdiscord_game_sdk.dylib $(LIB_DIR)/discord_game_sdk.dylib
endif
ifeq ($(UNAME),Linux)
	@echo "Unzipping discord_game_sdk.so"
	unzip $(DISCORD_SDK_ZIP) "lib/$(ARCH)/discord_game_sdk.so" -d $(LIB_DIR)
	mv $(LIB_DIR)/lib/$(ARCH)/discord_game_sdk.so $(LIB_DIR)/libdiscord_game_sdk.so
	rm -r $(LIB_DIR)/lib
	-ln -s $(LIB_DIR)/libdiscord_game_sdk.so $(LIB_DIR)/discord_game_sdk.so
endif

# TODO: Windows support	

# Default target
all: download $(LIB_OUTPUT) $(GO_OUTPUT)

# Build the shared library
$(LIB_OUTPUT): $(C_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Build the Go executable
$(GO_OUTPUT): $(GO_SRC) $(LIB_OUTPUT)
	$(GO) build -o $@ $(GO_LDFLAGS) $<

# Clean up build artifacts
clean:
	rm -f $(LIB_OUTPUT) $(GO_OUTPUT)

.PHONY: all download clean