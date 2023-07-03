# deploy-kubernetes

This brick provides *deployment* and *service* yaml files for deploying a container containing the service to a [kubernetes](https://kubernetes.io/)-cluster. The kubernetes-*deployment* makes sure that a predefined number of instances are up and running whereas the kubernetes-*service* provides a stable endpoint and uses a load balancer to forward traffic to the instances. Note that, the kubernetes service is of type *LoadBalancer* which exposes the service to the outside of your cluster.

> **_WARNING:_**  As the traffic may route subsequent calls to different instances, in order to get things working as expected, your sapper service must be **stateless**, i.e. no information that will be required across different requests must be stored in the service's memory. Use a cloud db such as e.g. [postgres](https://www.postgresql.org/) to make state persistent beyond a request.

## usage

Please make sure that you have the `kubectl` CLI tool installed on your system and properly configured so that it can access the kubernetes cluster your service shall be deployed to.

Then, the following command can be used to deploy the container to the cluster:
```bash
sapper service deploy .
```
This command will first make sure that the kubernetes *deployment* and *service* are in the cluster as described in the yaml files. Then an update of the container image is initiated.

## parameters

- **KUBERNETES_REPLICAS**: The desired number of replicas (i.e. number of running instances) in your kubernetes-*deployment*. default: 3.

- **PORT**: The port to be exposed. This has typically already been defined by the handler brick in case it had been added before.

## next steps

- For security reasons and others, you may want to change your service *type* to `ClusterIP` in your `service.yaml` so that your service can only be accessed from inside of the cluster. Then you might want to add an [ingress](https://kubernetes.io/docs/concepts/services-networking/ingress/) and configure the routing so that your service can access calls from outside of the cluster. 

- For adapting the number of running instances automatically based on the load, you may want to add an `autoscaler`. See, for example
[this walkthrow](https://kubernetes.io/docs/tasks/run-application/horizontal-pod-autoscale-walkthrough/) for more information.

## open issues

*-*
 