#pragma once 
#include "../Logger/Logger.h"
#include <map>
#include <typeindex>
#include <memory>
#include <list>
#include <functional>
#include "Event.h"



class IEventCallback{
    private:
        virtual void Call(Event& e) = 0;
    public:
        virtual ~IEventCallback() = default;
        void Execute(Event& e){
            Call(e);
        }
};

template<typename TOwner,typename TEvent>
class EventCallback: public IEventCallback{
    // callback funtion pointer that we need to invoke
    private:
    typedef void (TOwner::*CallbackFunction)(TEvent&);

    TOwner* ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event& e) override{
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
    }

    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction){
            this->ownerInstance = ownerInstance;
            this->callbackFunction = callbackFunction;

            
        }
        virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus{
    private:
        std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
    public:
        EventBus(){
            Logger::Log ("EventBus constructor called");
        }
        ~EventBus(){
            Logger::Log ("EventBus destructor called");
        }
        // Clears the Subscriber list

        void Reset(){
            subscribers.clear();
        }




        //////////////////////////////////////////////////
        //Subscribe to an event type<T>
        //in our implementation, a listner subscribe to an event
        //Example: eventBus->SubscribeToEvent<collsionEvent>(&Game::onCollsion)
        //////////////////////////////////////////////////
        template<typename TEvent, typename TOwner>
        void SubscribeToEvent(TOwner* ownerInstance, void(TOwner::*callbackFunction)(TEvent&)){
            if(!subscribers[typeid(TEvent)].get()){
                subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
            subscribers[typeid(TEvent)]->push_back(std::move(subscriber)); 
        }


        //////////////////////////////////////////////////
        //Emit an event type<T>
        //in our implementation, as soon as something emit an event we go ahead and
        // execute all the listner callback function 
        //Example: eventBus->EmitEvet<collsionEvent>(player enemy)
        //////////////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args){
            auto handlers = subscribers[typeid(TEvent)].get();
            if(handlers){
                for(auto it = handlers->begin(); it != handlers->end(); it++){
                    auto handler = it->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->Execute(event);
                }
            }
        }
};