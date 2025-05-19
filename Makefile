


git_version :=$(shell git --git-dir="$(1)/.git" describe --always --dirty --match "v*")



$(warning $(git_version ./))