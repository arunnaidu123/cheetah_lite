@page getting-started Getting Started with  Cheetah Pipelines

# Pipeline Launchers
Cheetah supports several different processing pipelines.
There is a seperate pipeline launcher executable for each type of data
to be processed:

| Executable                 | Input Data                | Location in Build Tree(relative to the build root directory) |
-----------------------------|---------------------------|--------------------------------------------------------------|
| cheetah_pipeline           | Beamformed Time Frequency | ./cheetah/pipeline/cheetah_pipeline                          |
| cheetah_candidate_pipeline | Pulsar Candidate Lists    | ./cheetah/candidate_pipeline/cheetah_candidate_pipeline      |

Each executable has its own help and usage information available by running it with the `--help` option.

Following is a brief summary of the common, most used options you should be familiar with to get going.

## Selecting a processing pipeline
Each executable allows you to select a specific data processing pipeline with the `-p <pipeline_name>`  option.

To see which pipelines are available run the required executable with the `--list-pipelines` option.

## Selecting a data source
To see the available data sources use the `--list-sources` option.\n
To select a source use the `-s <source_name>` option.

You may need to supply other parameters to actually use a selected data source.
For example a source that reads data from a file will need a filename.

## Configuration Files
Each pipeline launcher has a specific configuration file format.
The exepcted format of these files, along with a description of the various settings
are available with the `--help-config-file` option\n
You can specify which config file to use with `--config <config_filename>`

### Defining Processing Pools
The configuration file allows you to define one or more resource pools containing threads, or hardware accelerators.
Once a pool is defined you can use the configuration file to specify which pool should be used by any processing module.
In the way you can control exactly which hardware resources are to be dedicated to which part of the processing.
Any resources not assigned to a specifc pool will be put in the @b default pool and will be available to all parts of the pipeline.

Note that in addition to assinging pools to processing modules, you can also assign a priority level for jobs from that module to be run in that pool.

TODO - example

## Output Channels
Data is streamed out of the pipeline through output channels.
Different parts of the pipeline will broadcast data to a named channel and this data will be
fed to any exporter that is listening into that channel.

### Initialising Exporters
TODO

### Associating Exporters With Output Channels
Once an exporter has been initialised it needs to be assigned to one or more channels on which to listen out for data.
TODO
