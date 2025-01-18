package main

import (
	"discord-lsp/handlerDoc"

	"C"

	"github.com/tliron/commonlog"
	_ "github.com/tliron/commonlog/simple"
	"github.com/tliron/glsp"
	protocol "github.com/tliron/glsp/protocol_3_16"
	"github.com/tliron/glsp/server"
)

const isName = "Discord RPC Language Server or some shit"

var handler protocol.Handler
var version = "0.0.1"

func main() {
	commonlog.Configure(2, nil)

	handler = protocol.Handler{
		Initialize:  initialize,
		Initialized: initialized,
		Shutdown:    shutdown,
		SetTrace:    setTrace,
		// Will use this for changing the currently open file in Discord info
		TextDocumentDidOpen:  handlerDoc.TextDocumentDidOpen,
		TextDocumentDidClose: handlerDoc.TextDocumentDidClose,
	}

	server := server.NewServer(&handler, isName, true)

	server.RunStdio()
}

func initialize(context *glsp.Context, params *protocol.InitializeParams) (any, error) {
	commonlog.NewInfoMessage(0, "Initialize server")

	capabilities := handler.CreateServerCapabilities()
	capabilities.CompletionProvider = &protocol.CompletionOptions{}

	return protocol.InitializeResult{
		Capabilities: capabilities,
		ServerInfo: &protocol.InitializeResultServerInfo{
			Name:    isName,
			Version: &version,
		},
	}, nil
}

func initialized(context *glsp.Context, params *protocol.InitializedParams) error {
	return nil
}

func shutdown(context *glsp.Context) error {
	return nil
}
func setTrace(context *glsp.Context, params *protocol.SetTraceParams) error {
	protocol.SetTraceValue(params.Value)
	return nil
}
