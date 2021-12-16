# Sample Publisher App

Publisher apps consists of :
1. cpp publisher
   - Ubuntu
   - Alpine
   - Fedora
2. go publisher 
   - Ubuntu
   - Alpine
   - Fedora
3. python publisher
   - Ubuntu

1. **cpp publisher**: 
CPP publisher and server are supported in ubuntu, alpine and fedora will be running inside independent containers `ia_ubuntu_cpp_sample_pub`, `ia_alpine_cpp_sample_pub`, `ia_fedora_cpp_sample_pub` respectively.

2. **go publisher**: 
Go publisher and server are supported in ubuntu, alpine and fedora will be running inside independent containers `ia_ubuntu_go_sample_pub`, `ia_alpine_go_sample_pub`, `ia_go_cpp_sample_pub` respectively.

3. **python publisher**: 
Python publisher and server are supported only in ubuntu will be running inside independent container `ia_ubuntu_python_sample_pub`.

**NOTE:** Above publisher containers comprises of both publisher and server functionality.

The high level logical flow of Sample Publisher App is as below:

   1. Publisher contacts to ETCD service, using the [cfgmgr](https://github.com/open-edge-insights/eii-core/blob/master/common/libs/ConfigMgr/src/cfgmgr.c)
      library. It fetches it's private key and allowed client's public keys. Then it creates
      the publisher object and starts publishing the sample data at given topic
      (topic name : publish_test).

   2. Server contacts to ETCD service, and fetches it's private key and allowed client's public keys.
      Then it creates a server object according to given service name and waits for the client's request.
