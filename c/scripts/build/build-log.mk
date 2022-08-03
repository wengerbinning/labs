






rule_error_log   = @echo -en "\e[31m[$(strip $(1))] \e[0m"; echo "$(strip $(2))"
rule_warning_log = @echo -en "\e[34m[$(strip $(1))] \e[0m"; echo "$(strip $(2))"
rule_notice_log  = @echo -en "\e[33m[$(strip $(1))] \e[0m"; echo "$(strip $(2))"
rule_note_log    = @echo -en "\e[32m[$(strip $(1))] \e[0m"; echo "$(strip $(2))"
rule_info_log    = @echo -en "\e[32m[$(strip $(1))] \e[0m"; echo "$(strip $(2))"
rule_debug_log   = @echo -en "\e[35m[$(strip $(1))] \e[0m"; echo "$(strip $(2))"
# usage: rule_log <level> <message>
define rule_log
	$(if $(findstring $(strip $(1)), error, warning, notice, info, debug),
		$(call rule_$(strip $(1))_log, $2, $3),
		$(call rule_info_log, $1, $2))
endef



define make_log
	ifeq ($(strip $(1)), error)
		$(error $2)
	else
		$(warning $2)
	endif
endef