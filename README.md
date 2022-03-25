# Sample apps

This section provides more information about the Open Edge Insights (OEI) sample apps and how to use the core libraries packages like Utils, Message Bus, and ConfigManager in various flavors of Linux such as Ubuntu and Alpine operating systems or docker images for programming languages such as C++, Go, and Python.

The following table shows the details for the supported flavors of Linux operating systems or docker images and programming languages that support sample apps:

| Linux Flavor | Language         |
| :----------: | :--------------: |
| `Ubuntu`     | `C++, Go, Python`|
| `Alpine`     | `C++, Go`        |

The sample apps are classified as `publisher` and `subscriber` apps. For more information, refer to the following:

- [Publisher](publisher/README.md)
- [Subscriber](subscriber/README.md)

## Run the samples apps

For default scenario, the sample custom UDF containers are not the mandatory containers to run. The `builder.py` script runs the `sample-apps.yml` from the [build/usecases](https://github.com/open-edge-insights/eii-core/blob/master/build/usecases) directory and adds all the sample apps containers. Refer to the following list to view the details of the sample apps containers:

```yml
    AppContexts:
    # CPP sample apps for Ubuntu and Alpine operating systems or docker images
    - Samples/publisher/cpp/ubuntu
    - Samples/publisher/cpp/alpine
    - Samples/subscriber/cpp/ubuntu
    - Samples/subscriber/cpp/alpine

    # Python sample apps for Ubuntu operating systems or docker images
    - Samples/publisher/python/ubuntu
    - Samples/subscriber/python/ubuntu

    # Go sample apps for Ubuntu and Alpine operating systems or docker images
    - Samples/publisher/go/ubuntu
    - Samples/publisher/go/alpine
    - Samples/subscriber/go/ubuntu
    - Samples/subscriber/go/alpine
```

1. To run the `sample-apps.yml` file, run the following command:

    ```sh
    cd [WORKDIR]/IEdgeInsights/build
    python3 builder.py -f ./usecases/sample-apps.yml file used>
    ```

2. Refer to the [`Build OEI stack`](https://github.com/open-edge-insights/eii-core/blob/master/README.md) and the [`Run OEI service`](https://github.com/open-edge-insights/eii-core/blob/master/README.md) sections to build and run the sample apps.
