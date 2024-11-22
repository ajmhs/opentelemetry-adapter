# OpenTelemetry Adapter

TODO: Finish this file, it will become the basis of the case+code

## Overview
The OpenTelemetry Gateway is a service that collects, processes, and exports telemetry data from a DDS Topic. It is designed to be highly scalable and flexible, supporting multiple backends and data formats.

## Features
- **Data Collection**: Collects telemetry data from a DDS Topic.
- **Data Processing**: Processes data to ensure it is in the correct format.
- **Data Export**: Exports data to an OpenTelemetry collector.
- **Scalability**: Designed to handle large volumes of data.


## Installation
To install the OpenTelemetry Adapter, follow these steps:

Install the library, or a symlink to the library in one of the paths used by ld.
To view the paths, try running 
```bash
ld --verbose | grep SEARCH_DIR | tr -s ' ;' \\012
```
Usually 
```bash
cd /usr/local/lib
sudo ln -s {path to liobrary}
```
will be sufficient




1. Clone the repository:
    ```bash
    git clone https://github.com/ajmhs/opentelemetry_adapter.git
    ```
2. Navigate to the project directory:
    ```bash
    cd opentelemetry_adapter
    ```
3. Install dependencies:
    ```bash
    npm install
    ```

## Dependencies
To install [OpenTelemetry C++](https://github.com/open-telemetry/opentelemetry-cpp) locally using CMake, following these steps:

0. Install the pre-reqs for building opentelemetry with OTLP

```bash
apt install libcurlpp-dev
```

1. Clone the OpenTelemetry C++ GitHub repository to your local machine.

```bash
git clone https://github.com/open-telemetry/opentelemetry-cpp.git
```

2. Change your working directory to the OpenTelemetry C++ SDK directory.

```bash
cd opentelemetry-cpp
```

3. Create a build directory and navigate into it.

```bash
mkdir build
cd build
```

pro-tip: If you're on Linux, add this to your ~/.bashrc - it creates a bash function called mkcd which allows the make and cd to happen as a single command. i.e: ```mkcd build```
```bash
mkcd() { mkdir -p "$1" && cd "$1"; }
```

4. In the build directory run CMake, to configure and generate the build system without enabling tests:

```bash
cmake -DBUILD_TESTING=OFF -DWITH_PROMETHEUS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..
```

Or, if the cmake --build fails, you can also try:
```bash
cmake -DBUILD_TESTING=OFF -DWITH_ABSEIL=ON -DWITH_PROMETHEUS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..
```

5. Execute the build process:

```bash
cmake --build .
```

6. Install OpenTelemetry C++ in this directory (opentelemetry_adapter)

```bash
cmake --install . --prefix ../../opentelemetry_adapter/otel-cpp
```

## Building
```bash
mkcd build
cmake ..
cmake --build .
```


## Usage
To start the OpenTelemetry Gateway, run the following command:
```bash
npm start
```

## Configuration
Add a new job to the Observability platform's prometheus configuration in {$HOME}/rti_workspace/7.3.0/user_config/observability/prometheus/prometheus.yml

Configuration options can be set in the `config.json` file. Below is an example configuration:
```yaml
scrape_configs:
  #
  # Configuration for Prometheus exporter in a system
  #      
  # The job name is added as a label `job=<job_name>` to any timeseries scraped from this config.
  #
  - job_name: 'prometheus'

    # metrics_path defaults to '/metrics'
    # scheme defaults to 'http'.

    static_configs:
    - targets: 
      - localhost:9090 # Prometheus metrics
      
  - job_name: 'routing_service'      
    static_configs:
    - targets:
      - localhost:9464 # Application metrics provided by the routing service adapter
```
Save and (re)start the observability platform:
```bash
rtiobservability -t && rtiobservability -i
```

## Contributing
Contributions are welcome! Please read the [contributing guidelines](CONTRIBUTING.md) for more information.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact
For any questions or issues, please open an issue on GitHub or contact the maintainers.
