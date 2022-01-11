# Sample Subscriber App

Subscriber apps consists of :
1. cpp subscriber
   - Ubuntu
   - Alpine
   - Fedora
2. go subscriber 
   - Ubuntu
   - Alpine
   - Fedora
3. python subscriber
   - Ubuntu

1. **cpp subscriber**: 
CPP subscriber and client are supported in ubuntu, alpine and fedora will be running inside independent containers `ia_ubuntu_cpp_sample_sub`, `ia_alpine_cpp_sample_sub`, `ia_fedora_cpp_sample_sub` respectively.

2. **go subscriber**: 
Go subscriber and client are supported in ubuntu, alpine and fedora will be running inside independent containers `ia_ubuntu_go_sample_sub`, `ia_alpine_go_sample_sub`, `ia_go_cpp_sample_sub` respectively.

3. **python subscriber**: 
Python subscriber and client are supported only in ubuntu will be running inside independent container `ia_ubuntu_python_sample_sub`.

**NOTE:** Above subscriber containers comprises of both subscriber and client functionality.

The high level logical flow of Sample App is as below:

   1. Subscriber contacts to ETCD service using the [cfgmgr](https://github.com/open-edge-insights/eii-core/blob/master/common/libs/ConfigMgr/src/cfgmgr.c)
      library. It fetches it's private and public key and public key of publisher. It then creates
      the subscriber object (which subscribes to the same topic : publish_test) and start receiving
      the data and prints the same onto a console.

   2. Client contacts to ETCD service, and fetches its private key and public key of server. Then
      it creates a client object and sends a request to the server and gets the response back.