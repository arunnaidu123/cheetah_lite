ExporterType : a tag class to mark a std::string as an identifier for an exporter

template DataExport  : the main class that exports data, being configured with all the other classes - actually creates the exporters via its own factory

+------------------+
| DataExportConfig |
+------------------+
         < >
          |
          | 0..n
+------------------------+
| DataExportStreamConfig |       maps the configuration to a specific exporter factory tag
+------------------------+
