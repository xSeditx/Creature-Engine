#include"Core/Common.h"
#include"Core/Application.h"
#include"Layer.h"
 
Layer::Layer(const std::string& _name)
    :
    LayerName(_name)
{}
Layer::~Layer() {}

void Layer::OnAttach() {}
void Layer::OnDetach() {}
void Layer::OnUpdate() {}
bool Layer::OnEvent(Event _msg) { return false; }

//void Layer::OnEvent(Event& _msg) {}

// Tells if current layer is enabled or not 
bool Layer::is_Enabled() { return Enabled _EQUALS_ true; }

layerStack::layerStack()  {}
layerStack::~layerStack() {}

void layerStack::push(Layer* _layer)
{ 
    Stack.emplace_back(_layer);
}
bool layerStack::remove(Layer* _layer) { return true; }

void layerStack::push_back(Layer* _layer) { Stack.emplace_back(_layer); }
Layer* layerStack::pop_back() {
    Layer *result = Stack.back();
    Stack.pop_back();
    return result;
}

void layerStack::push_Overlay(Layer* _layer) {/* Add this layer to the Overlay stack of this specified layer */ };// Overlays.emplace_back(new emplace_back(_layer); }
bool layerStack::remove_Overlay(Layer*) { return true; }

void layerStack::CreateOverlays() {}
void layerStack::DeleteOverlays() {}

std::vector<Layer*>::iterator layerStack::begin() { return Stack.begin(); }
std::vector<Layer*>::iterator layerStack::end() { return Stack.end(); }

