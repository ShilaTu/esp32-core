################################################################################
###                        MAKEFILE HELP TEMPLATE                            ###
################################################################################
# This Makefile template can be used to qickly generate help messages for make.
# It is based on dynamic variables (see make manual)
#
# THIS MAKEFILE NEEDS TO BE INCLUDED AFTER ALL DECLARATIONS!
#
# own targets should be added to the 'TARGETS' variable, so help can find them.
# I.e. to add your target 'bar', do
# TARGETS += bar
#
# if a target has subtargets, you can add them to 'TARGETS_<target>' to
# indicate them as subtarget.
# I.e. to add your target 'bar-foo' to 'bar', do
# TARGETS_bar += bar-foo
#
# you may set the HELP variable to print a header in the help output
# I.e. HELP := This Makefile does something
#
# for a target specific help text you have to set the variable 'HELP_<target>'
# I.e. HELP_bar := helptext for bar
#
# if you want to define variables, you have to create a variable group by
# adding the group's name to the variable 'VARIABLES'
# I.e to add the variable group 'BUILDOPTION', do
# VARIABLES        += BUILDOPTION
# HELP_BUILDOPTION := build options
#
# you can now add variables to your new group
# I.e to add the variable 'CXX' to the group 'BUILDOPTION', do
# VARIABLE_BUILDOPTION += CXX
# HELP_CXX             := what compiler to use
#

.PHONY: help
help:
	@echo "--- help ---"
	@[ -n "$(HELP)" ] && echo -e "$(HELP)\n" || true;
	@$(foreach \
		target, \
		$(TARGETS), \
		[ -n "$(HELP_$(target))" ] && echo "make $(target)"         || true; \
		[ -n "$(HELP_$(target))" ] && echo -e "\t$(HELP_$(target))" || true; \
	)
	@echo -e "\n--- further help ---"
	@$(foreach \
		target, \
		$(TARGETS), \
		[ -n "$(TARGETS_$(target))" ] && echo -e "make help-$(target)\n\thelp for $(target) subtargets" || true; \
		$(foreach \
			subtarget, \
			$(TARGETS_$(target)), \
		) \
	)
	@$(foreach \
		group, \
		$(VARIABLES), \
		[ -n "$(VARIABLES_$(group))" ] && echo -e "\n--- $(HELP_$(group)) ---"  || true; \
		$(foreach \
			variable, \
			$(VARIABLES_$(group)), \
			echo "$(variable)"; \
			[ -n "$(HELP_$(variable))" ] && echo -e "\t$(HELP_$(variable))" || true; \
			echo -e "\t(currently: $($(variable)))"; \
		) \
	)

.PHONY: help-%
help-%:
	@echo "--- help for target $(@:help-%=%) ---"
	@[ -n "$(HELP)" ] && echo -e "$(HELP)\n" || true;
	@[ -n "$(TARGETS_$(@:help-%=%))" ] && echo -e "\n--- $(@:help-%=%) targets ---"  || true;
	@$(foreach \
		target, \
		$(TARGETS_$(@:help-%=%)), \
		[ -n "$(HELP_$(target))" ] && echo "make $(target)"         || true; \
		[ -n "$(HELP_$(target))" ] && echo -e "\t$(HELP_$(target))" || true; \
	)