package version

import "fmt"


func versionTO(code string) string {
	message := fmt.Sprintf("Version: %v", code)
	return message
}