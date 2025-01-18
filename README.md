# Discord Rich Presence through LSP (Language Server Protocol)

Adds discord rich presence to any text editor that supports LSP

## Getting started

### Dependencies
* golang
* clang
* make
* Discord Game SDK

### Building

```make all``` 
Will download the Discord Game SDK, unzip it and then build the binary (Linux and MacOS for now, Windows support welcome in a PR)

### Usage

After building the binary, either add it to your path or to your editor's config folder and link to it in the editor's config (or however it works with your editor when it comes to LSP)