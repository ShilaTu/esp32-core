# Project structure & rules



---
## General project structure
- The project consists of *components*
- *Components* reside inside the root of the repository
- All *component* directories should feature a README.md
- All *component* directories should feature a Makefile

```
lifesensor
│
├── README.md                 : main README.md of the project
├── Makefile                  : Makefile executing targets in all components
│
├── CONTRIBUTING.md           : information on how to contribute
├── LICENSE.md                : license information
│
├── docs
│   ├── README.md             : list & description of project documents
│   ├── Makefile              : Makefile building/generating documents
│   └── ...  
│
├── <component name>          : component name
│   ├── README.md             : general description of the component,
│   │                           its structure and a list of all subcomponents
│   ├── Makefile              : Makefile executing targets in all subcomponents
│   │
│   ├── <subcomponent name>   : subcomponent name
│   │   ├── README.md         : general description of the subcomponent
│   │   │                       and list of all subcomponent variants
│   │   ├── Makefile          : Makefile bundeling all variants
│   │   │
│   │   ├── docs
│   │   │   ├── README.md     : list & description of subcomponent documents
│   │   │   ├── Makefile      : Makefile building/generating documents
│   │   │   └── ...  
│   │   │
│   │   ├── <variant>         : a variant is a non-exchangeable component
│   │   │   ├── README.md     : general description of the variant
│   │   │   ├── Makefile      : Makefile integrating component toolchain
│   │   │   │
│   │   │   ├── docs
│   │   │   │   ├── README.md : list & description of variant documents
│   │   │   │   ├── Makefile  : Makefile building/generating documents
│   │   │   │   └── ...  
│   │   │   └── ...
│   │   │
│   │   ├── <variant>
│   │   │   └── ...
│   │   └── ...  
│   │
│   ├── <subcomponent name>
│   └── ...  
│
├── <component name>
│   └── ...  
└── ...  
```

---
## README.md
- A README.md should link to all subdirectories that contain a README.md
- Links to directories should always end with a slash (`/`)
  - I.e `[dir/](./dir/)`
- Links should always have a relative prefix (`./`,`../`, ... )
  - I.e `[file.txt](./file.txt)`
- All README.md's should describe important Makefile targets
- All README.md's should describe the structure of subdirectories

---
## Makefiles
- All Makefiles should support the common targets.
  - `help`
    - show information about targets and variables
  - `all`
    - run common jobs (like `build` & `test`)
  - `check`
    - run all check jobs
    - *check* jobs are environment validations
    - I.e. check if toolchain is installed
  - `setup`
    - run all setup jobs
    - *setup* jobs are environment configurations
    - I.e. define devices to flash
  - `build`
    - run all build jobs
    - should not be dependent on any `setup` target
  - `test`
    - run all test jobs
    - should not be dependent on any `setup` target
  - `clean`
    - delete everything build
  - `distclean`
    - delete everything generated
    - should reset the state prior execution of any target
- All Makefiles should integrate Makefiles in subdirectories.
- Targets that fall in one of these target categories should be named
`<category>-<target>`.
  - I.e. `build-library`, `clean-library`, `test-library`, ...
- A Makefile should include the [.common.mk](./.make/common.mk) Makefile
to define these common targets.
  - simply add `include $(shell git rev-parse --show-toplevel)/.make/common.mk`
as last statement in the Makefile.
