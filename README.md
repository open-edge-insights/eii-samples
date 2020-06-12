# Sample Applications for EIS Platform #

There are three sample apps for EIS Platform:
1. ### cpp_sample_app ###
    Description: [README.md](cpp_sample_app/README.md)
2. ### go_sample_app ###
    Description: [README.md](go_sample_app/README.md)
3. ### python_sample_app ###
    Description: [README.md](python_sample_app/README.MD)

**1. Creating Symbolic Links for the Apps:**

Symbolic links need to be created for publisher and subscriber directories of each app in order to comply with the rest of the EIS apps
where each directory has it's own Dockerfile, docker-compose.yml file and config.json.
This can be done by following commands:

```sh
cd ..
ln -s Samples/cpp_sample_app/publisher CppPublisher
ln -s Samples/cpp_sample_app/subscriber CppSubscriber
```
The same can be done for Go and Python Sample Apps.

**2. Refer [README.md](../README.md) to provision, build and run the Sample Apps.**
