# LLRP Reader Application

This repo provides the source code for an application which interacts with an LLRP reader, receives it's reports and sends them to a server.

## Installing Dependencies

Some of the project's dependencies include xml2, zlib, ssl and curl libraries. For Debian/Ubuntu based distros, the development pacakges for those libraries can be installed by running the following command:

```
$ sudo apt install zlib1g libxml2-dev libcurl4-openssl-dev libssl-dev
```                                                        	

Each package can also be installed seperatly.

## Building

First, the `build` and `bin` directories should be present, in order for the build process to work properly. If they're not already present, run the following in the project's root to create them:

```                                                        	
$ mkdir build bin
```                                                        	
                                                           	
To only build the project, run the following command from t	he project's root:
                                                           	
```                                                        	
$ make
```                                                        	
                                                           	
After running the command, the object files will be built under the `build` directory and the application's executable will be provided under `bin/main`.
                                                           	
## Running                                                 	
                                                           	
The project can be run manually by executing the `main` executable at the `bin` directory, providing the a JSON configuration file. This repo provides already a configuration file under `./config.json`, with all the configuration options. Currently, all the options must be provided in order for the application to run properly.
                                                           	
From the project's root, the following can be executed to run the application:

```
$ ./bin/main <confi-file-path>
```

The following command can also be run from the project's root, rebuilding source code changes, if any were detected, and running the project using included `config.json` file:

```
$ make run
```

As stated earlier, if the project wasn't yet built, make sure the `bin` and `build` directories are under the project's root directory, and if not create them, before running this command.

## Configuration Options
 
All the configuration objects must be provided in order to run the application properly.

- **readings-buffer-size** (integer) - Specifies the size of the buffer for storing readings.
- **crossings-buffer-size** (integer) - Defines the size of the buffer for storing crossings data.
- **session-data-file** (string) - Specifies the filename or path where session data is stored in JSON format.
- **reader-host** (string) - Indicates the IP address or hostname of the reader device.
- **enable-impinj-extensions** (boolean) - Determines if Impinj-specific extensions should be enabled.
- **reset-to-factory-defaults** (boolean) - Controls whether to reset the reader settings to factory defaults.
- **connection-attempt-timeout** (integer) - Timeout duration (in seconds) for attempting to establish a connection.
- **access-report-timeout** (integer) - Timeout duration (in seconds) for receiving the next access report.
- **inventory-duration** (integer) - Duration (in seconds) for an inventory operation.
- **readings-export-destination** (string) - Destination path or filename for exporting readings data in JSON format.
- **server-url** (string) - URL of the server or API endpoint to which to send the crossings.
- **station-name** (string) - Name or identifier of the station.
- **crossings-export-destination** (string) - Destination path or filename for exporting crossings data in JSON format.
- **data-processing-logs** (string) - Output destination for data processing logs ("stdout").
