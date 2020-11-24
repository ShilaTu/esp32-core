
-include .template.mk

ifdef TEMPLATING_CHOICES

VARIABLES += TEMPLATING
HELP_TEMPLATING := Templating options

VARIABLES_TEMPLATING    += TEMPLATING_CHOICES
HELP_TEMPLATING_CHOICES := existing templates

VARIABLES_new            += TEMPLATING_COMMON
HELP_TEMPLATING_COMMON   := Common template options

VARIABLES_TEMPLATING_COMMON += NAME
HELP_NAME                   := how to rename the template
NAME                        ?= #empty

TEMPLATING_SUFFIXES += m4
TEMPLATING_SUFFIXES += envsubst

define TEMPLATE_NEW_TEMPLATING_TARGET

$$(if $$(TEMPLATING_PATH_$1),,$$(error Variable 'TEMPLATING_PATH_$1' is not set in .template.mk))
$$(if $$(TEMPLATING_HELP_$1),,$$(error Variable 'TEMPLATING_HELP_$1' is not set in .template.mk))

VARIABLES_new         += new-$1-vars
VARIABLES_new-$1-vars := $$(TEMPLATING_VARS_$1)
HELP_new-$1-vars      := 'new-$1' template options

TEMPLATING_M4_$1       := $$(foreach DEFINE,$$(VARIABLES_TEMPLATING_COMMON) $$(TEMPLATING_VARS_$1),-D__$$(DEFINE)__=$$($$(DEFINE)))
TEMPLATING_ENVSUBST_$1 := $$(foreach DEFINE,$$(VARIABLES_TEMPLATING_COMMON) $$(TEMPLATING_VARS_$1),$$$${$$(DEFINE)} )

TEMPLATING_INPUT_$1  := $$(foreach PATH,$$(TEMPLATING_PATH_$1),$$(shell find $$(PATH) -type f))
TEMPLATING_FILES_$1  := $$(foreach PATH,$$(TEMPLATING_PATH_$1), $$(shell find $$(PATH) -type f -printf '%P\n') )
TEMPLATING_INDEX_$1  := $$(shell for i in {1..$$(words $$(TEMPLATING_FILES_$1))}; do echo $$$$i; done)
TEMPLATING_COPY_$1   := $$(shell echo '$$(addprefix $$(NAME)/, $$(TEMPLATING_FILES_$1))' | m4 $$(TEMPLATING_M4_$1))
TEMPLATING_OUTPUT_$1 := $$(shell echo "$$(TEMPLATING_COPY_$1) " | sed -E $$(foreach SUFFIX,$$(TEMPLATING_SUFFIXES), -e 's/([^\S]+)\.$$(SUFFIX)/\1/g'))


.PHONY: _check-new-$1
_check-new-$1:
	$$(foreach \
		VAR, \
		$$(VARIABLES_TEMPLATING_COMMON) $$(TEMPLATING_VARS_$1), \
		$$(if \
			$$($$(VAR)), \
			, \
			$$(info ERROR: option $$(VAR) is missing. please specify via '$$(VAR)=<value>'. see 'make help-new' for more information) $$(error ) \
		) \
	)

.PHONY: new-$1
TARGETS_new += new-$1
HELP_new-$1 := $$(TEMPLATING_HELP_$1)
new-$1: $$(TEMPLATING_OUTPUT_$1) | _check-new-$1

$$(NAME)/%:$$(NAME)/%.m4
	@mkdir -p $$(dir $$@)
	m4 $$(TEMPLATING_M4_$1) -D__FILENAME__=$$(notdir $$@) -D__DIRNAME__=$$(dir $$@) $$^  > $$@

$$(NAME)/%:$$(NAME)/%.envsubst
	@mkdir -p $$(dir $$@)
	FILENAME=$$(notdir $$@) DIRNAME=$$(dir $$@) $$(foreach DEFINE,$$(VARIABLES_TEMPLATING_COMMON) $$(TEMPLATING_VARS_$1),$$(DEFINE)=$$($$(DEFINE))) \
	envsubst '$$(TEMPLATING_ENVSUBST_$1) $$$${FILENAME} $$$${DIRNAME}' < $$^  > $$@


define TEMPLATE_RULE_COPY
.INTERMEDIATE: $$2
$$2:$$1
	@mkdir -p $$(dir $$2)
	cp $$1 $$2
endef

.SECONDARY: $$(TEMPLATING_OUTPUT_$1)

$$(foreach \
	INDEX, \
	$$(TEMPLATING_INDEX_$1), \
	$$(eval \
		$$(call \
			TEMPLATE_RULE_COPY, \
			$$(word $$(INDEX), $$(TEMPLATING_INPUT_$1)), \
			$$(word $$(INDEX), $$(TEMPLATING_COPY_$1)) \
		) \
	) \
)

endef

$(foreach \
	TEMPLATE, \
	$(TEMPLATING_CHOICES), \
	$(eval $(call TEMPLATE_NEW_TEMPLATING_TARGET,$(TEMPLATE))) \
)

.PHONY:new
TARGETS_common  += new
HELP_new  := create something new from a template
DOCKER_NO += new
new: | help-new

endif
