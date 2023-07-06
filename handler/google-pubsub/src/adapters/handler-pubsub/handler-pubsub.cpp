#include "handler-pubsub.h"
#include <microservice-essentials/context.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/request/request-processor.h>
#include <microservice-essentials/utilities/metadata-converter.h>
#include <nlohmann/json.hpp>
#include <google/cloud/pubsub/subscriber.h>
#include <google/cloud/status.h>


namespace {

using json = nlohmann::json;

void from_json(const json& entity_json, Entity& e)
{    
    entity_json.at("id").get_to(e.id);
    entity_json.at("string_property").get_to(e.string_property);
    entity_json.at("int_property").get_to(e.int_property);
}

}

PubSubHandler::PubSubHandler(Api& api, const std::string& google_project_id, const std::string& subscription_id)
    : _api(api)
    , _google_project_id(google_project_id)
    , _subscription_id(subscription_id)
{
}

PubSubHandler::~PubSubHandler()
{
}

void PubSubHandler::Handle()
{
  std::unique_lock<std::mutex> lock(_stop_mutex);
  MSE_LOG_INFO(std::string("listening on subscription ") + _subscription_id + " of project " + _google_project_id);

  namespace pubsub = ::google::cloud::pubsub;
  pubsub::Subscription s(_google_project_id, _subscription_id);    
  std::shared_ptr<pubsub::SubscriberConnection> connection = pubsub::MakeSubscriberConnection(s);
  pubsub::Subscriber subscriber(connection);

  auto future_status = subscriber.Subscribe(
    [&](pubsub::Message const& m, pubsub::AckHandler h) {         
      
      //1. acknowledge message so that it won't be sent again
      std::move(h).ack();
      
      //2. process message (ignore result)
      setEntity(m.data(), mse::ToContextMetadata(m.attributes()));      
    });
    
  _stop_cv.wait(lock, [this](){ return _stop_requested == true;});
  future_status.cancel();
  
  google::cloud::Status status = future_status.get();
  std::stringstream statusStream;
  statusStream << "finished listening to pubsub with status: " << status;
  MSE_LOG_INFO(statusStream.str());
}

void PubSubHandler::Stop()
{
  MSE_LOG_INFO("stop requested");    
  {
    std::lock_guard lk(_stop_mutex);
    _stop_requested = true;
  }
  _stop_cv.notify_one();    
}


void PubSubHandler::setEntity(const std::string& message, const mse::Context::Metadata& metadata)
{
  mse::RequestHandler("setEntity", mse::Context(metadata))
      .Process([&](mse::Context&)
      {      
          Entity e;
          try
          {
              from_json(json::parse(message), e);
          }
          catch(const json::exception& error)
          {
              throw std::invalid_argument(error.what());                
          }            
          _api.SetEntity(e);
          return mse::Status::OK;
      });
}
