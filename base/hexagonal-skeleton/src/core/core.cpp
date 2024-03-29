#include "core.h"
#include <stdexcept>


Core::Core(EntityRepo* repo, EntityNotifier* notifier)
   : _repo(repo), _notifier(notifier)
{
}

void Core::SetEntity(const Entity& entity)
{
   if(!_repo) {
      throw std::runtime_error("invalid repo");
   }

   if(entity.id.empty()) {
      throw std::invalid_argument("invalid id");
   }
   

   _repo->Store(entity);

   if(_notifier){
      _notifier->EntityChanged(entity);
   }
}
 
 std::optional<Entity> Core::GetEntity(const std::string& id) const
 {
   if(!_repo) {
      throw std::runtime_error("invalid repo");
   }

   if(id.empty()) {
      throw std::invalid_argument("invalid id");
   }
   return _repo->Retrieve(id);    
 }
 