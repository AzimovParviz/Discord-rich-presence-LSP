package handlerDoc

import (
	"fmt"
	"net/url"
	"os"
	"unsafe"

	"github.com/tliron/glsp"
	protocol "github.com/tliron/glsp/protocol_3_16"
)

//#include "../discord/setActivity.h"
//#include <stdlib.h>
import "C"

//export convertToCString
func convertToCString(s string) *C.char {
	C.CString(s)
	defer C.free(unsafe.Pointer(C.CString(s)))
	return C.CString(s)
}

func TextDocumentDidOpen(context *glsp.Context, params *protocol.DidOpenTextDocumentParams) error {
	TextDocument := params.TextDocument
	uri := TextDocument.URI
	langID := TextDocument.LanguageID
	filename, err := url.PathUnescape(uri)
	if len(filename) > 6 {
		filename = filename[7:]
	} else {
		return fmt.Errorf("filename is too short to remove first 6 characters: %v", filename)
	}
	if err != nil {
		return fmt.Errorf("failed to unescape URI: %v", err)
	}

	fmt.Println("Filename:", filename)
	file, err := os.Create(fmt.Sprintf("%s.txt", filename))

	C.setActivity(convertToCString(filename), convertToCString(langID))
	if err != nil {
		return fmt.Errorf("failed to create file: %v", err)
	}
	defer file.Close()

	return nil
}

func TextDocumentDidClose(context *glsp.Context, params *protocol.DidCloseTextDocumentParams) error {

	C.setActivity(convertToCString("Idle"), convertToCString("empty"))

	return nil
}
