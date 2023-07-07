# handler-google-pubsub

This handler calls the service's *SetEntity* whenever it receives a google [pubsub](https://cloud.google.com/pubsub) message containing the entity as JSON from a pubsub subscription.

The message's attributes are forwarded to the request's context as metadata. This can e.g. be used for authorization or tracing.

The brick uses **google-cloud-cpp** for pulling the pubsub messages and **nlohmann_json** for creating entities from json.

## usage

First, you need to create google pubsub [subscription](https://console.cloud.google.com/cloudpubsub/subscription) for your google pubsub topic in your google project. 

Then, you need to grant your service access to that subscription. This can be done by creating a [service account](  https://console.cloud.google.com/iam-admin/serviceaccounts). Add the role *Pub/Sub Subscriber* to it. Make sure to add this service account to the permission list of that pubsub subscription. Follow the principle of least priviledge. Then, create a key for the service account and download it in JSON format (`key.json`). Keep this key safe. Consider using *google secret manager* or something similar to store the key. The `GOOGLE_APPLICATION_CREDENTIALS` environment variable needs to point to this file. When running the `sapper service run .` command to launch your service, the environment variable is automatically being set.

To test the pubsub handler, you can publish a message to the pubsub topic of your description using the following command in google cloud shell:

```bash
gcloud pubsub topics publish <my_topic_name> --message '{ "id":"test", "string_property":"abc", "int_property":12 }'
```

## parameters

 -  **GOOGLE_PROJECT**: The google cloud project id in which the topic has been created. The command `gcloud projects list` can be run in google cloud shell to obtain the project name. 
  - **PUBSUB_SUBSCRIPTION**: The google pubsub subscription id from which the messages shall be consumed. The command `gcloud pubsub subscriptions list` can be used in google cloud shell to obtain a list of subscriptions. 
  - **GOOGLE_APPLICATION_CREDENTIALS**: The path in the filesystem where the *key.json* is located (see above for more details).

## next steps

Create a second service with a *notifier-google-pubsub* brick to send pubsub messages whenever an entity has been changed. 

## open issues

*-* 
