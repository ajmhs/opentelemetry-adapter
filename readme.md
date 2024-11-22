# OpenTelemetry Gateway

## Overview
The OpenTelemetry Gateway is a service that collects, processes, and exports telemetry data from a DDS Topic. It is designed to be highly scalable and flexible, supporting multiple backends and data formats.

## Features
- **Data Collection**: Collects telemetry data from a DDS Topic.
- **Data Processing**: Processes data to ensure it is in the correct format.
- **Data Export**: Exports data to an OpenTelemetry collector.
- **Scalability**: Designed to handle large volumes of data.


## Installation
To install the OpenTelemetry Gateway, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/ajmhs/opentelemetry_gateway.git
    ```
2. Navigate to the project directory:
    ```bash
    cd opentelemetry_gateway
    ```
3. Install dependencies:
    ```bash
    npm install
    ```

## Dependencies
To install [OpenTelemetry C++](https://github.com/open-telemetry/opentelemetry-cpp) locally using CMake, following these steps:

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
cmake -DBUILD_TESTING=OFF ..
```

Or, if the cmake --build fails, you can also try:
```bash
cmake -DBUILD_TESTING=OFF -DWITH_ABSEIL=ON ..
```

5. Execute the build process:

```bash
cmake --build .
```

6. Install OpenTelemetry C++ in this directory (opentelemetry_gateway)

```bash
cmake --install . --prefix ../../opentelemetry_gateway
```

## Building


## Usage
To start the OpenTelemetry Gateway, run the following command:
```bash
npm start
```

## Configuration
Configuration options can be set in the `config.json` file. Below is an example configuration:
```json
{
  "collector": {
    "enabled": true,
    "endpoint": "http://localhost:4317"
  },
  "processor": {
    "batch_size": 100,
    "timeout": 5000
  },
  "exporter": {
    "backend": "prometheus",
    "endpoint": "http://localhost:9090"
  }
}
```

## Contributing
Contributions are welcome! Please read the [contributing guidelines](CONTRIBUTING.md) for more information.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact
For any questions or issues, please open an issue on GitHub or contact the maintainers.
