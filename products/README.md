# LifeSensor products
*Products* are supported [*LifeSensor*](https://lifesensor.org) devices consisting of [*parts*](../parts/).

---
## Products
- [prototype/](./prototype/)
  - prototype & testing ground of various parts

---
## Products structure

```
lifesensor/products
│
├── README.md              : mandatory README.md you are currently reading
├── Makefile               : mandatory Makefile executing targets in all subdirs
│
├── <product name>         : mandatory unique name of the product
│   ├── README.md          : mandatory general description of the product
│   │                        and elaboration of differences between version
│   │
│   ├── <product version>  : mandatory unique product version
│   │   ├── README.md      : mandatory version specific description of the product,
│   │   │                    should list all features
│   │   │
│   │   ├── Makefile       : mandatory Makefile for integration of the product
│   │   │
│   │   ├── docs           : optional parent directory for documents
│   │   │   │                i.e. documentation, datasheets
│   │   │   ├── README.md  : description of documents
│   │   │   └── ...
│   │   │
│   │   └── requirements   : mandatory parent directory for requirements
│   │       │                used by requirement management tools
│   │       ├── README.md  : description of requirements
│   │       └── ...
│   │
│   ├── <product version>
│   │   └── ...
│   └── ...
│
├── <product name>
│   └── ...
└── ...
```

---
## Makefile
<!-- LIST OF MAKEFILE TARGETS -->
- `make help`
  - show Makefile options
- `make all`
  - build & test all products
- `make build`
  - build all products
- `make test`
  - test all products
- `make clean`
  - clean all products
- `make distclean`
  - distclean all products

---
## Adding new products
- If you want to add a new product,
please copy the [*template product*](./.template/) and modify your copy.

---
## Rules
- *Products* of the [*LifeSensor*](https://lifesensor.org) project should have a dedicated directory here.
- *Products* should not include [*parts*](../parts/) or other *products*
- *Products* may compete for the same functionality.
  - Different *products* may choose different [*parts*](../parts/) to achieve the same
- *Products* should follow the directory scheme defined by the [*template product*](./.template/)
  - *Parent directories* may contain any structure
  - Optional *parent directories* can be removed
- Only non-breaking changes may be introduced as patch for a *product*
- *Products versions* denote non-backwards-compatible variants of the same *product*
  - If major implementations change, consider creating a new *product* or *product version*
- *Products versions* should never denote the state of the *product*
  - All *product versions* should provide their intended functionality
