#pragma once


#include"Core/Common.h"
#include"Core/Application.h"
#include"Core/EventSystem.h"

class CREATURE_API Layer
{
public:
	Layer(const std::string& _name = "Default_Layer");
	virtual ~Layer();

	virtual void OnAttach();
	virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnRender();

    /* If The Layer Desires to Handle this return True */
    virtual bool OnEvent(Event _msg);
    
    /* Returns the Debug name of our current Layer */
    inline const std::string& g_Name() const { return LayerName; }

	/* Tells if current layer is enabled or not */
    bool is_Enabled();

    EventHandler Observer;
    
private:
    bool Enabled{ false };
    std::string LayerName{};
};

class CREATURE_API layerStack
{
public:
	layerStack();
	~layerStack();

	void push(Layer* _layer);
	bool remove(Layer* _layer);

	void push_back(Layer* _layer);
	Layer* pop_back();
    Layer* back() { return Stack.back(); }

	void push_Overlay(Layer* _layer);
	bool remove_Overlay(Layer*);

	void CreateOverlays();
	void DeleteOverlays();

    std::vector<Layer*>::iterator begin();
    std::vector<Layer*>::iterator end();

	layerStack* Overlays;

    template<typename _Ty>
    _Ty* get_Layer_Object(_Ty *_obj)
    {
        return this->_obj;
    }
private:
	std::vector<Layer*> Stack;
	std::vector<Layer*>::iterator Inserter_iter;
};


