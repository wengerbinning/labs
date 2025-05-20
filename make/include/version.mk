git_repo = $(shell git rev-parse --is-inside-work-tree 2>/dev/null)
git_desc_options = --always --tags --dirty --match "v*"
git_desc = $(shell git describe $(git_desc_options) 2>/dev/null)


version=$(file <version)

ifeq ($(version),)
version=$(if $(git_desc),$(git_desc),unknown)
endif

$(warning $(version))