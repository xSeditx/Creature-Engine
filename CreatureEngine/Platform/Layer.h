#include"EventSystem.h"
 

class CREATURE_API Layer
{
public:
	Layer(const std::string& _name = "Default_Layer");
	virtual ~Layer();

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate();
	virtual void OnEvent(Event& _msg);

	/* Returns the Debug name of our current Layer */
	inline const std::string& g_Name() const { return LayerName; }

	/* Tells if current layer is enabled or not */
	bool is_Enabled() { return Enabled _EQUALS_ true; }
private:
	bool Enabled;
	std::string LayerName;
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

	void push_Overlay(Layer* _layer);
	bool remove_Overlay(Layer*);

	void CreateOverlays();
	void DeleteOverlays();
	std::vector<Layer*>::iterator begin() { return Stack.begin(); }
	std::vector<Layer*>::iterator end() { return Stack.end(); }

	layerStack* Overlays;
private:
	std::vector<Layer*> Stack;
	std::vector<Layer*>::iterator Inserter_iter;

};
