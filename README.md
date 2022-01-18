# EII Sample Apps

EII Sample Apps demonstrates the usage of core libraries packages like Utils, EIIMessageBus and ConfigManager in different flavors of Linux, i.e., Ubuntu, Alpine and Fedora in cpp, go and python.

Below table shows the support of sample apps on different flavors of linux on different programming languages

| Linux Flavor | Language         |
| :----------: | :--------------: |
| `Ubuntu`     | `CPP, GO, PYTHON`|
| `Alpine`     | `CPP, GO`        |
| `Fedora`     | `CPP, GO, PYTHON`|

These Sample Apps are segregated into publisher and subscriber and below are its respective README.md:
1. [publisher](publisher/README.md)
2. [subscriber](subscriber/README.md)

## Steps to run EII samples apps

Please find the ordered steps for running Sample Apps.

1. As per EII default scenario, the sample custom UDF containers are not mandatory containers to run, hence the builder.py should run `sample-apps.yml` present in the [build/usecases](https://github.com/open-edge-insights/eii-core/blob/master/build/usecases) directory. All the sample apps containers are added in this example. 
Below code snnipet signifies the same. 

```yml
    AppContexts:
    # CPP sample apps for ubuntu, fedora and alpine
    - Samples/publisher/cpp/ubuntu
    - Samples/publisher/cpp/fedora
    - Samples/publisher/cpp/alpine
    - Samples/subscriber/cpp/ubuntu
    - Samples/subscriber/cpp/fedora
    - Samples/subscriber/cpp/alpine

    # Python sample apps for ubuntu and fedora
    - Samples/publisher/python/ubuntu
    - Samples/subscriber/python/ubuntu
    - Samples/publisher/python/fedora
    - Samples/subscriber/python/fedora

    # Go sample apps for ubuntu, fedora and alpine
    - Samples/publisher/go/ubuntu
    - Samples/publisher/go/fedora
    - Samples/publisher/go/alpine
    - Samples/subscriber/go/ubuntu
    - Samples/subscriber/go/fedora
    - Samples/subscriber/go/alpine
```

2. With the above pre-requisite done, please run the below command:

    ```sh
    cd [WORKDIR]/IEdgeInsights/build
    python3 builder.py -f ./usecases/sample-apps.yml file used>
    ```

3. Refer [README.md](https://github.com/open-edge-insights/eii-core/blob/master/README.md) to build and run the Sample Apps
