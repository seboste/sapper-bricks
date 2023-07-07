# repo-google-cloud-storage

Repo that stores and retrieves the service's example entities to and from a [google cloud storage](https://cloud.google.com/storage) bucket in a JSON file format. Each entity gets its own google cloud storage object. The name corresponds to entity's id.

The brick uses **google-cloud-cpp** for communicating with google cloud storage and **nlohmann_json** for converting entities to and from json.

## usage

First, you need to create google cloud storage [bucket](https://console.cloud.google.com/storage/browser) in your google project. 

Then, you need to grant your service access to that bucket. This can be done by creating a [service account](  https://console.cloud.google.com/iam-admin/serviceaccounts). Add the role *Storage Admin* to it. Make sure to add this service account to the permission list of that cloudstorage bucket. Follow the principle of least priviledge. Then, create a key for the service account and download it in JSON format (`key.json`). Keep this key safe. Consider using *google secret manager* or something similar to store the key. The `GOOGLE_APPLICATION_CREDENTIALS` environment variable needs to point to this file. When running the `sapper service run .` command to launch your service, the environment variable is automatically being set.

## parameters

  - **CLOUD_STORAGE_BUCKET**: The google cloud storage bucket that shall be used to store the entities in. The command `gsutil ls` can be used in google cloud shell to obtain a list of buckets. 
  - **GOOGLE_APPLICATION_CREDENTIALS**: The path in the filesystem where the *key.json* is located (see above for more details).

## next steps

*-*

## open issues

Google cloud storage object names have [restrictions](https://cloud.google.com/storage/docs/objects). Neither, does the brick restrict the entity id to the set of possible names, nor are the invalid characters escaped.
