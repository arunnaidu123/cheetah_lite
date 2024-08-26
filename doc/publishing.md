# API Documentation Publishing

The API documents are published automatically to gitlab pages on a push to the master branch of the repository.

This process is driven by the file *.gitlab-ci.yml* which defines a pipeline of jobs that are run on a *'runner'*. Runners run in *docker* containers, and the YAML file defines how this container is set up for each job.

To publish the documentation there are 2 stages to the pipeline:

1. **Build**: cmake configures the doxygen file - There is a *documentation* build type so that only documentation build dependencies are required. Doxygen then runs to create the documentation.
2. **Deploy**: The html pages produced by doxygen are published on *gitlab page*.

Runners are set up in the project settings - we currently use shared runners provided by gitlab - make sure they are enabled in the _CI/CD Pipeline_ section of the project settings. **Note** this can mean that there is a wait for resources when the pipeline runs.

## Published documentation

The API documentation after publishing is available at this address:

[https://ska-telescope.gitlab.io/pss/ska-pss-cheetah](https://ska-telescope.gitlab.io/pss/ska-pss-cheetah)

## Issues

It would appear that sometimes the *publish* stage of the pipeline fails, or takes a long time to run - likely because it uses a shared runner, and this may get very overloaded. If this happens it appears that this can prevent further pushes to master branch. In this situation it is best to cancel the job.

To do this go to *commits* on the gitlab dashboard - there will be an icon indicating the state of the CI piple (Could be 'fail' or 'running' or 'passed'). Click on this icon to display the jobs. From here it is possible to either 'cancel' and job or re-run it.

## Useful Links

[Gitlab Pages Documentation](https://docs.gitlab.com/ce/user/project/pages/index.html)

[.gitlab-ci.yml Documentation](https://docs.gitlab.com/ce/ci/yaml/README.html)

[Gitlab CI Documentation](https://docs.gitlab.com/ce/ci/README.html)
