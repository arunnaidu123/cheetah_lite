@page run-pipeline Running Cheetah Pipelines

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
Each pipeline launcher is configured using a configuration (config) file of a specific format. The config file contains parameters, such as the expected data source, data sinks, beam parameters (where applicable) and specific configuration for each of the required modules (e.g., DDTR, SPS, etc). The expected format of these files, along with a description of the various settings are available with the `--help-config` option, e.g.,

@code
./cheetah_pipeline --help-config
@endcode

will print an example config file in XML format, showing the required parameters and a help message for each one.  Config files in JSON format will also be accepted by the pipeline launchers.  In order to run a pipeline launcher with the required configuration, the user can specify which config file to use with `--config <config_filename>`

To show a list of configurable modules, we can run (e.g.),

@code
./cheetah_pipeline --list-modules
@endcode

and specific configuration information for any of these modules (and their submodules) can be shown using a command of the form,

@code
./cheetah_pipeline --help-module <module>
./cheetah_pipeline --help-module <module>.<submodule>
@endcode

### Running a simple cheetah_pipeline

In the following, an example is shown of the simplest configuration of the cheetah_pipeline launcher, in which time-frequency data from a single beam is received (by way of a sigproc (filterbank) file) from the beamformer, and the data is simply written to disk via an export channel (see below). No other operations on the input data, by other modules, are enabled.

@code
<?xml version="1.0" encoding="utf-8"?>
<cheetah>
  <beams>
    <beam>
      <id>first_beam</id>
      <active>true</active>
      <cpu>
        <affinity>0,1,2,3,4,5</affinity>
      </cpu>
      <sinks>
        <threads>2</threads>
        <channels>
          <raw>
            <active>true</active>
            <sink>
              <id>sigproc_file</id>
            </sink>
            <id>channel_raw</id>
          </raw>
        </channels>
        <sink_configs>
          <sigproc>
            <spectra_per_file>937500</spectra_per_file>
            <dir>sink</dir>
            <extension>.fil</extension>
            <id>sigproc_file</id>
          </sigproc>
        </sink_configs>
      </sinks>
      <source>
        <sigproc>
          <file>source/input.fil</file>
          <chunk_samples>39063</chunk_samples>
        </sigproc>
      </source>
    </beam>
  </beams>
</cheetah>
@endcode

A beam is a view of a patch on the sky. SKA telescopes are widely separated in comparison to the size of each individual element and this creates a narrow beam on the sky. To maximise survey speed, a large number of beams are formed by the beamformer. Each beam observes a different patch of sky and will be searched individually for pulsars and radio transients. Properties, such as the origin of the beamformed data (i.e., a port and IP address from which the data from a particular beam is expected to arrive) or the filenames, sizes and location of data products that will be produced by the search, can be configured individually for each of the three beams, using the <beam> tag, which requires parameters for the following submodules, as shown by `--help-module`, e.g.,

@code
./cheetah_pipeline --help-module beams.beam

Name
    cheetah_pipeline - cheetah pipeline launcher module : beams.beam

  --beams.beam.active   enable this beam
Submodules:
  More help is available on the following (--help-module <module_name>)
    beams.beam.cpu
    beams.beam.sinks
    beams.beam.source
@endcode

In the above example XML file, <cpu>, <sinks> and <source> are configured, as well as <id> (defining a specific beam identifier) and <active> (defining whether or not, data from beam <id> is to be processed).

#### Configuring the data source

<source>, defines the source of the data stream that is to be processed by the pipeline. In this case, the source is a sigproc (filterbank) file,  for which the path is given, as well as the number of samples to parse in a single chunk. Alternatively, we could configure the cheetah pipeline to receive data from a UDP stream by setting the source to e.g., `udp_low` and setting the required parameters, such as an IP address and port from which to listen for beamformed data, e.g.,

@code
./cheetah_pipeline --help-module beams.beam.source.udp_low.listen
Name
    cheetah_pipeline - cheetah pipeline launcher module : beams.beam.source.udp_low.listen

  --beams.beam.source.udp_low.listen.port arg (=34345)
                                        the port of the endpoint
                                        (default=34345)
  --beams.beam.source.udp_low.listen.ip_address arg (=127.0.0.1)
                                        the endpoint IP address
                                        (default=127.0.0.1)
@endcode

#### Configuring the data sinks

Following processing, output data products from a processing pipeline are exported via export channels. Different parts of the pipeline will broadcast data to a named export channel and this data will be fed to any exporter that is listening on that channel. Under the channels submodule, destinations (sinks) for specific data product types are defined. In the config example above, an export channel is defined called 'raw' that routes data to a single sink, specifically for sigproc files (the format of the data product the pipeline will write to disk).  The sink is specifically configured using one of number of pre-defined sink configs whose id, ('sigproc_file', in this case) corresponds to the appropriate sink id. A list of available sink configs can be shown with the command below

@code
./cheetah_pipeline --help-module beams.beam.sinks.sink_configs
Name
    cheetah_pipeline - cheetah pipeline launcher module : beams.beam.sinks.sink_configs

Submodules:
  More help is available on the following (--help-module <module_name>)
    beams.beam.sinks.sink_configs.ocld_files
    beams.beam.sinks.sink_configs.scl_files
    beams.beam.sinks.sink_configs.sigproc
    beams.beam.sinks.sink_configs.sp_candidate_data
    beams.beam.sinks.sink_configs.spccl_files
    beams.beam.sinks.sink_configs.spccl_sigproc_files
    beams.beam.sinks.sink_configs.spccl_spead
@endcode

In the case above, the <sigproc> config is used, which requires parameters including the number of spectra per output file, the directory (must exist already) into which the output files will be written, and the file extensions to use (the filename is defined according to the current date and time).

Running the pipeline as follows, will read the input file from disk, and write the contents to one or more output files of the same format, each containing a maximum number of spectra according to beams.beam.sinks.sink_configs.sigproc.spectra_per_file.

@code
./cheetah_pipeline --config=<path_to_config_file> -p Empty -s sigproc
@endcode

where `-p` specifies the pipeline type required (see `--list-pipelines`) and `-s` specifies the data source.

### Running a simple cheetah_candidate_pipeline

The cheetah candidate pipeline receives candidate data (as produced by a cheetah_pipeline). The expected config file format can be shown using the `--help-config` option.

The simplest example of a cheetah_candidate_pipeline configuration is, like the cheetah_pipeline example above, one which simply receives and then stores some data to disk. Below is shown an example of such a configuration, utilising a data source and a sink.

@code

<?xml version="1.0" encoding="UTF-8"?>
<cheetah_candidate_pipeline>
  <sinks>
    <threads>3</threads>
    <channels>
      <raw_candidates>
        <active>true</active>
        <sink>
           <id>candidate_files</id>
        </sink>
      </raw_candidates>
    </channels>
    <sink_configs>
      <sp_candidate_data>
        <extension>.spccl</extension>
        <dir>/tmp</dir>
        <id>candidate_files</id>
      </sp_candidate_data>
    </sink_configs>
  </sinks>
  <sources>
    <SpeadReader>
      <cpu>
        <devices>3</devices>
      </cpu>
      <listen>
        <port>9021</port>
        <ip_address>0.0.0.0</ip_address>
      </listen>
    </SpeadReader>
  </sources>
</cheetah_candidate_pipeline>
@endcode

The source, in this case, is a spead2 stream (spead2 is an implementation of the spead protocol - a streaming protocol for the exchange of astronomical data), parameterised by an IP address from which data is expected to arrive, and the specific port to listen on.  The sunks and sink configs are configured similarly to the case for the cheetah_pipeline, where a channel called `raw_candidates` is defined and given the id `candidate_files`. The sink is configured using a sink_config called `sp_candidate_data` to write files with the extension `.spccl` to the directory `/tmp`.

### Defining Processing Pools
The configuration file allows you to define one or more resource pools containing threads, or hardware accelerators.
Once a pool is defined you can use the configuration file to specify which pool should be used by any processing module.
In the way you can control exactly which hardware resources are to be dedicated to which part of the processing.
Any resources not assigned to a specifc pool will be put in the @b default pool and will be available to all parts of the pipeline.

Note that in addition to assinging pools to processing modules, you can also assign a priority level for jobs from that module to be run in that pool.

TODO - example

### Initialising Exporters
TODO

### Associating Exporters With Output Channels
Once an exporter has been initialised it needs to be assigned to one or more channels on which to listen out for data.
TODO
