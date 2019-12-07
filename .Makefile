.DEFAULT_GOAL=default

HELP_default = make $(DEFAULT)
.PHONY: default
default: $(DEFAULT)

HELP_all = make $(ALL)
.PHONY: all
all: $(ALL)

HELP_check = make $(CHECK)
.PHONY: check
check: $(CHECK)

HELP_setup = make $(SETUP)
.PHONY: setup
setup: $(SETUP)

HELP_clean = make $(CLEAN)
.PHONY: clean
clean: $(CLEAN)

HELP_distclean = make clean $(DISTCLEAN)
.PHONY: distclean
distclean: clean $(DISTCLEAN)

.PHONY: help
help:
	@echo "--- help ---"
	@[ -n "$(DEFAULT)" ]   && echo -e "make          \n\t$(HELP_default)"   || true;
	@[ -n "$(ALL)" ]       && echo -e "make all      \n\t$(HELP_all)"       || true;
	@[ -n "$(CHECK)" ]     && echo -e "make check    \n\t$(HELP_check)"     || true;
	@[ -n "$(SETUP)" ]     && echo -e "make setup    \n\t$(HELP_setup)"     || true;
	@[ -n "$(CLEAN)" ]     && echo -e "make clean    \n\t$(HELP_clean)"     || true;
	@[ -n "$(DISTCLEAN)" ] && echo -e "make distclean\n\t$(HELP_distclean)" || true;
	@echo
	@$(foreach \
		target, \
		$(TARGET), \
		echo "make $(target)"; \
		[ -n "$(HELP_$(target))" ] && echo -e "\t$(HELP_$(target))" || true; \
	)
	@$(foreach \
		target, \
		$(TARGET), \
		[ -n "$(TARGET_$(target))" ] && echo -e "\n--- $(target) sub targets ---"  || true; \
		$(foreach \
			subtarget, \
			$(TARGET_$(target)), \
				echo "make $(subtarget)"; \
				[ -n "$(HELP_$(subtarget))" ] && echo -e "\t$(HELP_$(subtarget))" || true; \
		) \
	)
	@[ -n "$(VARIABLE)" ] && echo -e "\n--- VARIABLES ---"  || true;
	@$(foreach \
		variable, \
		$(VARIABLE), \
		echo "$(variable)"; \
		[ -n "$(HELP_$(variable))" ] && echo -e "\t$(HELP_$(variable))" || true; \
		echo -e "\t(currently: $($(variable)))"; \
	)
