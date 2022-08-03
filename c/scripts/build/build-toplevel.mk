
all:

include build-environment.mk

IGNORES := *.o .*.cmd .*.target
PHONY := prepare
prepare:
	-@test -d .target.old && rm -rf .target.old || true
	-@test -d .target && mv .target .target.old || true
	+@mkdir .target
	+@$(call environment, .environment)

$(shell test -f .targets && rm -f .targets)
$(foreach target, $(targets), $(call build_target, $(target)))
all: prepare $(targets)
	@echo '$(sort $(IGNORES))' | tr -s ' ' '\n' > .ignore

PHONY += clean
clean: $(foreach target, $(targets), $(target)-clean)
	$(if $(shell test -f .ignore && echo .ignore), \
		-rm -f $(shell cat .ignore | xargs), -rm -f $(IGNORES))
PHONY += dist-clean
dist-clean: clean
	-rm -rf .ignore .environment .target*

PHONY += help
help:
	-@echo "cat << EOF"  > .build-help.sh
	$(if $(shell test -f build-help.hdoc && echo build-help.hdoc),\
		-@cat build-help.hdoc >> .build-help.sh)
	-@$(ECHO) "\nEOF"        >> .build-help.sh
	-@bash .build-help.sh && rm -f .build-help.sh




.PHONY: $(PHONY) FORCE
.FORCE: FORCE