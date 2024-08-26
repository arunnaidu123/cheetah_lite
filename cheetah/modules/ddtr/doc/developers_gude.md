@page ddtr_developers_guide DDTR developers guide
# Notes for the developer
## Few important pointers for adding a new module
1. The user can in principle select multiple algorithms to perform a dedispersion transform. DDTR uses the CommonDedispersion object to estimate the number of samples and buffer overlap required for the DDTR algorithm. Every module should have a DedispersionPlan object with interface similar as given in cpu/DedispersionPlan.h.

2. Add the module to the top level as a template parameter to the DdtrAlgos in file ddtr/Ddtr.h

3. All the algorithms must pass the generic tests.
