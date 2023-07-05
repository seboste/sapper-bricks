# notifier-google-pubsub

Whenever, one of the example entities changes, this brick publishes a google [pubsub](https://cloud.google.com/pubsub) message containing the entity as JSON to a pubsub topic. Google pubsub subscriptions may be configured to consume the message. 

The brick uses **google-cloud-cpp** for publishing the pubsub message and **nlohmann_json** for converting entities to json.

## usage

First, you need to create google pubsub [topic](https://console.cloud.google.com/cloudpubsub/topic) in your google project. 

Then, you need to grant your service access to that topic. This can be done by creating a [service account](  https://console.cloud.google.com/iam-admin/serviceaccounts). Add the role *Pub/Sub Publisher* to it. Make sure to add this service account to the permission list of that pubsub topic. Follow the principle of least priviledge. Then, create a key for the service account and download it in JSON format (`key.json`). Keep this key safe. Consider using *google secret manager* or something similar to store the key. The `GOOGLE_APPLICATION_CREDENTIALS` environment variable needs to point to this file. When running the `sapper service run .` command to launch your service, the environment variable is automatically being set.

## parameters

 -  **GOOGLE_PROJECT**: The google cloud project id in which the topic has been created. The command `gcloud projects list` can be run in google cloud shell to obtain the project name. 
  - **PUBSUB_TOPIC**: The google pubsub topic id to which the message shall be published upon notification. The command `gcloud pubsub topics list` can be used in google cloud shell to obtain a list of topics. 
  - **GOOGLE_APPLICATION_CREDENTIALS**: The path in the filesystem where the *key.json* is located (see above for more details).

## next steps

Create a pubsub subscription for your topic in order to consume the notifications.

## open issues

The forward declaration of *PublisherConnection* in *notifier-pubsub.h* requires hardcoding the version of the *google-cloud-cpp* library. Upon updating the library, this needs to be adapted.
 