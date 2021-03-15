# Sample Applications for EII Platform #

There are three sample apps for EII Platform:
1. ### cpp_sample_app ###
    Description: [README.md](cpp_sample_app/README.md)
2. ### go_sample_app ###
    Description: [README.md](go_sample_app/README.md)
3. ### python_sample_app ###
    Description: [README.md](python_sample_app/README.MD)

**1. Creating Symbolic Links for the Apps:**

Symbolic links need to be created for publisher and subscriber directories of each app in order to comply with the rest of the EII apps
where each directory has it's own Dockerfile, docker-compose.yml file and config.json.
This can be done by following commands:

```sh
cd [repo]

ln -s Samples/cpp_sample_app/publisher CppPublisher
ln -s Samples/cpp_sample_app/subscriber CppSubscriber

ln -s Samples/go_sample_app/publisher GoPublisher
ln -s Samples/go_sample_app/subscriber GoSubscriber

ln -s Samples/python_sample_app/publisher PythonPublisher
ln -s Samples/python_sample_app/subscriber PythonSubscriber
```

**2. Running [builder.py](../build/builder.py) to generate consolidated docker-compose.yml and eii_config.json.**

```sh
 cd [repo]/build

 python3 builder.py
```

**3. Refer [provision/README.md](../README.md) to provision, build and run the Sample Apps.**
